#include "smtpsslsocket.h"
#include <QDebug>
//#include "smtp.h"
#include <QRegularExpression>

const QByteArray smtp_method_str[] = {"LOGIN","PLAIN"};

SmtpSSlSocket::SmtpSSlSocket(QObject *parent)
    : QSslSocket(parent)
    , m_auth_method(NO_FIND_METHOD)
    , m_last_state(ST_NONE)
{
    m_accaunt = 0;
}

void SmtpSSlSocket::smtp_connect(Account *account)
{
    m_accaunt = account;
    connectToHostEncrypted(account->hostName, account->nPort);
    connect(this,SIGNAL(readyRead()),this,SLOT(slotReadyRad()));
}

bool SmtpSSlSocket::checkAuthMethod(const QString &ansver)
{
    QList<QString> list = ansver.split("\r\n");

    for (int i = 0; i < list.count(); ++i)
    {
        if(list.at(i).contains("AUTH",Qt::CaseInsensitive))
        {
            if(authMethod(list.at(i)) != NO_FIND_METHOD)
            {
                return true;
            }
        }

    }

    return false;
}

SmtpSSlSocket::Auth_Method SmtpSSlSocket::authMethod(const QString &str)
{
    QList<QString> auth_list = str.split(QRegularExpression("[-\\s]"));

    for (int j = 0; j < COUNT_METHOD; ++j)
    {
        for (int i = 0; i < auth_list.count(); ++i)
        {
            QString name = auth_list[i];
            QString serch_str = smtp_method_str[j];

            if (name.contains(serch_str,Qt::CaseInsensitive))
            {
                m_auth_method = static_cast<Auth_Method>(j);
                return m_auth_method;
            }
        }
    }

    return NO_FIND_METHOD;
}

SmtpSSlSocket::State SmtpSSlSocket::decodeAnsver(const QString &ansv)
{
    State return_val = ST_ERROR;
    static int st = 0;
    st = st | m_last_state;

    //qDebug() << "S: " << ansv;

    switch (ansv.left(3).toInt()) {
    case 220:
        return_val = ST_HELLO;
        break;
    case 250:
        if(st == 0)
            return_val = ST_AUTH;
        else if(st == (ST_LOGIN_OK))
            return_val = ST_TO;
        else if(st == (ST_LOGIN_OK | ST_TO_OK))
            return_val = ST_DATA;
        else if(st == (ST_LOGIN_OK | ST_TO_OK | ST_DATA_OK))
            return_val = ST_END;
        break;
    case 334:
        return_val = ST_LOGIN;
        break;
    case 235:
        st = st | ST_LOGIN_OK;
        return_val = ST_FROM;
        break;
    case 354:
        st = st | ST_DATA_OK;
        return_val = ST_DATA_OK;
        break;
    default:
        return_val = ST_ERROR;
        break;
    }

    return return_val;

}

void SmtpSSlSocket::login(const QString &ansv)
{
    QByteArray out;

    switch (m_auth_method) {
    case COUNT_METHOD:
    case NO_FIND_METHOD:
        break;
    case PLAIN:
        //write(loginPlain(account)+"\r\n");
        break;
    case LOGIN:
        QString str = ansv.right(ansv.size() - 3);

        str = QByteArray::fromBase64(str.toUtf8());

        if(str.contains("USERNAME",Qt::CaseInsensitive))
        {
            out = m_accaunt->login.toUtf8().toBase64() + "\r\n";
            write(out);
        }
        else if(str.contains("PASSWORD",Qt::CaseInsensitive))
        {
            out = m_accaunt->passwd.toLocal8Bit()+"\r\n";
            write(out);
        }
        break;
    }
    qDebug() << "U: " << out;
}

void SmtpSSlSocket::slot_hello()
{
    QByteArray message("EHLO " +  m_accaunt->login.toLocal8Bit().split('@').at(0) + "\r\n");
    write(message);
    qDebug() << "U: " << message;

}

void SmtpSSlSocket::slot_auth(const QByteArray &ansv)
{
    if(checkAuthMethod(ansv))
    {
        QByteArray mess("AUTH " + smtp_method_str[m_auth_method] + "\r\n");
        qDebug() << "U: " << mess + "\r\n";
        write(mess);
    }
    else
    {
        qDebug() << "U: NO_FIND_METHOD";
    }
}

void SmtpSSlSocket::from()
{
    QByteArray mess("MAIL FROM: " + m_accaunt->login.toLocal8Bit() + "\r\n" );
    qDebug() << "U: " << mess;
    write(mess);
}

void SmtpSSlSocket::slotReadyRad()
{
    QByteArray arr =  readAll();
    qDebug() << arr;

    switch (decodeAnsver(arr)) {
    case ST_HELLO:
        slot_hello();
        break;
    case ST_AUTH:
        slot_auth(arr);
        break;
    case ST_LOGIN:
        login(arr);
        break;
    case ST_FROM:
        from();
        break;
    case ST_TO:
        emit sendRecipientsTo();
        break;
    case ST_DATA:
        m_last_state = ST_STRT_DAT;
        decodeAnsver("");
        break;
    case ST_DATA_OK:
        emit smtpSendData();
        break;
    default:
        break;
    }

}

void SmtpSSlSocket::slotSendRecipient(const QString &address)
{
    if (address.isEmpty())
    {
        write("DATA \r\n");
        m_last_state = ST_TO_OK;
        decodeAnsver("");
        return;
    }

    QString recipient = address;
    recipient.replace(QRegularExpression(".*<"),"");
    recipient.replace(QRegularExpression(">"),"");

    QByteArray mess("RCPT TO: " + recipient.toLocal8Bit() + "\r\n");

    write(mess);

}

void SmtpSSlSocket::slotSmtpSendData(const QByteArray &data, bool end_data)
{
    write(data);
    if (end_data)
        write("\r\n.\r\n");

}
