#include "smtp.h"
#include <QString>
#include <QSslSocket>
#include <QDebug>
#include <QRegularExpression>
#include <eml.h>
#include <QFile>

namespace Smtp {

//const QByteArray FIND_STRINGS[] = {"PLAIN","LOGIN"};
const QByteArray FIND_STRINGS[] = {"LOGIN","PLAIN"};


static Auth_Method auth_find_method = NO_FIND_METHOD;


bool checkAuthMethod(const QString &ansver)
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

void hello(QSharedPointer<Eml::Account> account, QSslSocket *sock)
{
    QByteArray message("EHLO " + account->login.split('@').at(0) + "\r\n");
    sock->write(message);
    qDebug() << "U: " << message;
    addLog("U: " + message);

}

Auth_Method authMethod(const QString &str)
{
    QList<QString> auth_list = str.split(QRegularExpression("[-\\s]"));

    for (int j = 0; j < COUNT_METHOD; ++j)
    {
        for (int i = 0; i < auth_list.count(); ++i)
        {
            QString name =auth_list[i];
            QString serch_str = FIND_STRINGS[j];

            if (name.contains(serch_str,Qt::CaseInsensitive))
            {
                auth_find_method = static_cast<Auth_Method>(j);

                return auth_find_method;
            }
        }
    }

    return NO_FIND_METHOD;

}


QByteArray findedAuthMethod()
{
    if(auth_find_method == NO_FIND_METHOD)
    {
        qDebug() << "U: NO_FIND_METHOD";
        addLog("U: NO_FIND_METHOD");
        return QByteArray();
    }

    qDebug() << "U: " << QByteArray("AUTH " + FIND_STRINGS[auth_find_method] + "\r\n");
    addLog("U: " +  QByteArray("AUTH " + FIND_STRINGS[auth_find_method]));

    return QByteArray("AUTH " + FIND_STRINGS[auth_find_method] + "\r\n") ;
}

Auth_Method authMethod()
{
    return auth_find_method;
}

void login(QSharedPointer<Eml::Account> account, QSslSocket *sock, const QString &ansv)
{
    QByteArray out;

    switch (auth_find_method) {
    case Smtp::PLAIN:
        sock->write(loginPlain(account)+"\r\n");
        break;
    case Smtp::LOGIN:
        QString str = ansv.right(ansv.size()-3);

        str = QByteArray::fromBase64(str.toUtf8());

        if(str.contains("USERNAME",Qt::CaseInsensitive))
        {
            out = account.data()->login.toBase64()+"\r\n";
            sock->write(out);
        }
        else if(str.contains("PASSWORD",Qt::CaseInsensitive))
        {
            out = account.data()->passwd.toBase64()+"\r\n";
            sock->write(out);
        }
        break;
    }
    qDebug() << "U: " << out;
    addLog("U:" + out);

}

QByteArray loginPlain(QSharedPointer<Eml::Account> account)
{
    QByteArray b_arr("_" + account.data()->login + "_" + account.data()->passwd);
    b_arr[0] = 0;
    b_arr[account.data()->login.size()+1] = 0;
    qDebug() << b_arr << ":" << b_arr.toBase64();

    addLog(b_arr + ":" + b_arr.toBase64());

    return b_arr.toBase64();
}

void addLog(const QString &log)
{
    QFile f("smtp.log");
    if(f.open(QIODevice::Append))
    {
        QTextStream log_stream(&f);

        log_stream  << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << ">> " <<log << "\r\n" ;

        f.close();
    }
}

Smtp::State decodeAnsver(const QString &ansv, State last_State)
{
    State return_val = ST_ERROR;
    static int st = 0;
    st = st | last_State;

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

}//namespace Smtp

