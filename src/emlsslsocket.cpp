#include <QAbstractSocket>
#include <QCryptographicHash>
#include <QSharedPointer>
#include <QSslSocket>
#include <QSslError>
#include <iostream>
#include <stdlib.h>
#include <QRegularExpression>
#include "eml.h"
#include "emlsslsocket.h"
#include "smtp.h"
#include <QQueue>
#include <QDataStream>

EmlSSlSocket::EmlSSlSocket(EmlMessage *mes, QObject *parent)
    : QObject(parent)
{
    m_p_account.reset(new Eml::Account());
    m_p_buffer = NULL;

    m_p_stream.reset();

    m_p_socket = NULL;
    m_nport = Eml::port(Eml::S_POP3);

    m_p_message = mes;
    m_recipiens = m_p_message->header().data()->to();

    connect(this,SIGNAL(signalError(QString)),this, SLOT(slotError(QString)));

    Smtp::addLog("====================================== new session =========================================");
}

EmlSSlSocket::~EmlSSlSocket()
{

	if (m_p_socket->isOpen())
	{
		m_p_socket->close();
	}
 		
}

void EmlSSlSocket::setAccount(Eml::Account *account)
{
    m_p_account.reset(account);
}


void EmlSSlSocket::setPort(int port)
{
    m_nport = port;
}

const QString &EmlSSlSocket::lastError()
{
    return m_lastError;
}

QSslSocket *EmlSSlSocket::socket()
{
    return m_p_socket;
}

QByteArray EmlSSlSocket::loginPlain()
{
    QByteArray b_arr("_" + m_p_account.data()->login + "_" + m_p_account.data()->passwd);
    b_arr[0] = 0;
    //b_arr[m_login.size()] = '\r';
    b_arr[m_p_account.data()->login.size()+1] = 0;

    qDebug() << b_arr << ":" << b_arr.toBase64();

    return b_arr.toBase64();
}

int EmlSSlSocket::sendMessage(EmlMessage * mess)
{
		m_p_message = mess;
        //if (m_p_socket->state() != QAbstractSocket::)
		return 0;
}

int EmlSSlSocket::writeSocket(const QByteArray & b_arr)
{
	return 0;
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

    if(m_p_account.data()->hostName.isEmpty() || m_nport == 0)
    {
        m_lastError = QString("invalid HostName[%1] or port[%2].")
                .arg(m_p_account.data()->hostName)
                .arg(m_nport);

        emit signalError("connect To Host Encrypted");
        return;
    }

    qDebug() << "connected to " << m_p_account.data()->hostName <<":" << m_nport;
    m_p_socket->connectToHostEncrypted(m_p_account.data()->hostName, m_nport);
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
//     static Smtp::State st = Smtp::ST_ERROR;
//     QString  ansv = QString::fromUtf8(m_p_socket->readAll());
// 
//     qDebug()<< "S: " << ansv;
//     Smtp::addLog("S: " + ansv);
// 
//     QByteArray tmp_arr;
// 
//     st = Smtp::decodeAnsver(ansv);
// 
// 
//     switch (st) {
//     case Smtp::ST_HELLO:
//         Smtp::hello(m_p_account, m_p_socket);
//         break;
//     case Smtp::ST_AUTH:
//         Smtp::checkAuthMethod(ansv);
//         m_p_socket->write(Smtp::findedAuthMethod());
//         break;
//     case Smtp::ST_LOGIN:
//         Smtp::login(m_p_account, m_p_socket, ansv);
//         break;
//     case Smtp::ST_FROM:{
//         tmp_arr = "MAIL FROM: " + m_p_account.data()->login + " \r\n";
//         m_p_socket->write(tmp_arr);
//     }
//     case Smtp::ST_TO:
//         if (!m_recipiens.isEmpty())
//         {
//             QString rcpt(m_recipiens.front());
//             m_recipiens.pop_front();
// 
//             rcpt.replace(QRegularExpression(".*<"),"");
//             rcpt.replace(QRegularExpression(">"),"");
//             tmp_arr = "RCPT TO: " + rcpt.toLatin1() + " \r\n";
// 
//             m_p_socket->write(tmp_arr);
// 
//             if(m_recipiens.isEmpty())
//                 Smtp::decodeAnsver("",Smtp::ST_TO_OK );
//         }
//         break;
//     case Smtp::ST_DATA:
//         tmp_arr = "DATA \r\n";
//         m_p_socket->write(tmp_arr);
//         Smtp::decodeAnsver("",Smtp::ST_STRT_DAT);
//         break;
//     case Smtp::ST_DATA_OK:
// 
//         m_p_buffer = new QString();
//         m_p_message->out_stream.reset(new QTextStream(m_p_buffer,QIODevice::ReadWrite));
//         m_p_message->createMessage();
// 
//         m_p_socket->write(m_p_buffer->toUtf8());
// 
//         delete m_p_buffer;
// 
//         m_p_socket->write(" \r\n.\r\n ");
// 
//         tmp_arr = " \r\n.\r\n ";
// 
// 
//         break;
//     case Smtp::ST_END:
//         tmp_arr = " QUIT \r\n";
//         m_p_socket->write("QUIT\r\n");
//         //m_p_socket->close();
//         break;
//     default:
//         break;
//     }
// 
//     if (!tmp_arr.isEmpty())
//     {
//         qDebug() << "U: " << tmp_arr;
//         Smtp::addLog("U: " + tmp_arr);
//     }


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

