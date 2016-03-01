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

EmlSSlSocket::EmlSSlSocket(QObject *parent)
    : QObject(parent)
    , m_account(Eml::Account())
{
    m_p_socket = NULL;
    m_nport = Eml::port(Eml::S_POP3);
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
    QString  ansv = QString::fromUtf8(m_p_socket->readAll());
    qDebug() << "S:" << ansv;
    Smtp::addLog(QString("S:" + ansv).replace(QRegularExpression("\r\n$"),""));

    if (ansv.contains("220"))
    {
        Smtp::hello(m_account, m_p_socket);
    }
    else if (ansv.contains("250"))
    {
        Smtp::checkAuthMethod(ansv);
//        if(!Smtp::findedAuthMethod().isEmpty() )
//        {
            m_p_socket->write(Smtp::findedAuthMethod());
//        }
    }
    else if (ansv.contains("334"))
    {
        Smtp::login(m_account, m_p_socket, ansv);
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

