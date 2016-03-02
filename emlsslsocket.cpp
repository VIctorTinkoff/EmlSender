#include <QAbstractSocket>
#include <QCryptographicHash>
#include <QSslSocket>
#include <QSslError>
#include <iostream>
#include <stdlib.h>
#include <QRegularExpression>
#include "eml.h"
#include "emlsslsocket.h"
#include "smtp.h"
#include <QQueue>

EmlSSlSocket::EmlSSlSocket(EmlMessage *mes, QObject *parent)
    : QObject(parent)
    , m_account(Eml::Account())
{
    m_p_socket = NULL;
    m_nport = Eml::port(Eml::S_POP3);

    m_p_message = mes;
    m_recipiens = m_p_message->to();

    connect(this,SIGNAL(signalError(QString)),this, SLOT(slotError(QString)));

    Smtp::addLog("====================================== new session =========================================");
}

EmlSSlSocket::~EmlSSlSocket()
{

}

void EmlSSlSocket::setAccount(const Eml::Account &account)
{
    m_account = account;
}


void EmlSSlSocket::setPort(int port)
{
    m_nport = port;
}

const QString &EmlSSlSocket::lastError()
{
    return m_lastError;
}

QByteArray EmlSSlSocket::loginPlain()
{
    QByteArray b_arr("_" + m_account.login + "_" + m_account.passwd);
    b_arr[0] = 0;
    //b_arr[m_login.size()] = '\r';
    b_arr[m_account.login.size()+1] = 0;

    qDebug() << b_arr << ":" << b_arr.toBase64();

    return b_arr.toBase64();
}

void EmlSSlSocket::updateEnabledStatate()
{

}

void EmlSSlSocket::sslConnect()
{
    if (!m_p_socket)
    {
        m_p_socket = new QSslSocket(this);

        connect(m_p_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(m_p_socket, SIGNAL(encrypted()),
                this,SLOT(socketEncripted()));
        connect(m_p_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,SLOT(socketError(QAbstractSocket::SocketError)));
        connect(m_p_socket, SIGNAL(sslErrors(QList<QSslError>)),
                this,SLOT(sslError(QList<QSslError>)));
        connect(m_p_socket, SIGNAL(readyRead()),
                this,SLOT(socketReadyRead()));
    }

    if(m_account.hostName.isEmpty() || m_nport == 0)
    {
        m_lastError = QString("invalid HostName[%1] or port[%2].")
                .arg(m_account.hostName)
                .arg(m_nport);

        emit signalError("connect To Host Encrypted");
        return;
    }

    qDebug() << "connected to " << m_account.hostName <<":" << m_nport;
    m_p_socket->connectToHostEncrypted(m_account.hostName, m_nport);
    //  m_p_socket->write("001 CAPABILITY\r\n");
}

void EmlSSlSocket::socketStateChanged(QAbstractSocket::SocketState /*st*/)
{
    // qDebug() << "socketStateChanged";
}

void EmlSSlSocket::socketEncripted()
{

    if (!m_p_socket)
        return;
    // qDebug() << "socket encripted";

    //    p_form->textEditSessionOutput->clear();
    //    p_form->lineEditInput->setFocus();
}

void EmlSSlSocket::socketReadyRead()
{
    static Smtp::State st = Smtp::ST_ERROR;

    QString  ansv = QString::fromUtf8(m_p_socket->readAll());
    qDebug()<< "S: " << ansv;
    QByteArray tmp_arr;

    st = Smtp::decodeAnsver(ansv);


    switch (st) {
    case Smtp::ST_HELLO:
        Smtp::hello(m_account, m_p_socket);
        break;
    case Smtp::ST_AUTH:
        Smtp::checkAuthMethod(ansv);
        m_p_socket->write(Smtp::findedAuthMethod());
        break;
    case Smtp::ST_LOGIN:
        Smtp::login(m_account, m_p_socket, ansv);
        break;
    case Smtp::ST_FROM:{
        tmp_arr = "MAIL FROM: " + m_account.login + " \r\n";
        qDebug() <<"U: " << tmp_arr ;
        m_p_socket->write(tmp_arr);
    }
    case Smtp::ST_TO:
        if (!m_recipiens.isEmpty())
        {
            QString rcpt(m_recipiens.front());
            m_recipiens.pop_front();

            rcpt.replace(QRegularExpression(".*<"),"");
            rcpt.replace(QRegularExpression(">"),"");

            tmp_arr = "RCPT TO: " + rcpt.toLatin1() + " \r\n";
            qDebug() <<"U:" << tmp_arr;
            m_p_socket->write(tmp_arr);

            if(m_recipiens.isEmpty())
                Smtp::decodeAnsver("",Smtp::ST_TO_OK );
            //send_state =Smtp::ST_DATA;
        }
        break;
    case Smtp::ST_DATA:
        tmp_arr = "DATA \r\n";
        qDebug() << "U: " << tmp_arr;
        m_p_socket->write(tmp_arr);
         Smtp::decodeAnsver("",Smtp::ST_STRT_DAT);
        break;
    case Smtp::ST_DATA_OK:
        m_p_socket->write(m_p_message->mesage().toLatin1());
        m_p_socket->write(" \r\n.\r\n ");
        qDebug() << "U: " << " \r\n.\r\n ";
        break;
    case Smtp::ST_END:
        qDebug() << "U: " << " QUIT \r\n";
        m_p_socket->write("QUIT\r\n");
        //m_p_socket->close();
        break;
    default:
        break;
    }

}

void EmlSSlSocket::socketSendData(const QString &message)
{
    m_p_socket->write(message.toUtf8() + "\r\n");
    //p_form->lineEditInput->clear();
}

void EmlSSlSocket::socketError(QAbstractSocket::SocketError err)
{
    QString stdError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError) ?
                "The host not found." : (err == QAbstractSocket::RemoteHostClosedError) ?
                    "The remote host is closed." : (err == QAbstractSocket::ConnectionRefusedError) ?
                        "The conection was refused." :  m_p_socket->errorString() ;

    qDebug() << stdError;
    Smtp::addLog(stdError);
    //qDebug() << "socketError" ;
}

void EmlSSlSocket::sslError(const QList<QSslError> &err_s)
{
    qDebug() << "sslError" << err_s ;
}

void EmlSSlSocket::slotError(const QString &err)
{
    std::cout << err.toStdString() << ": '"
              << m_lastError.toStdString()
              << "'" <<std::endl;
}

