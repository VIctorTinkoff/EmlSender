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

void hello(const Eml::Account &account, QSslSocket *sock)
{
    QByteArray message("EHLO " + account.login.split('@').at(0) + "\r\n");
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

void login(const Eml::Account &account, QSslSocket *sock, const QString &ansv)
{
    QByteArray out;

    switch (auth_find_method) {
    case Smtp::PLAIN:
        sock->write(loginPlain(account)+"\r\n");
        break;
    case Smtp::LOGIN:
        QString str = ansv;

        str.replace(QRegularExpression("334"),"");
        str = QByteArray::fromBase64(str.toUtf8());

        if(str.contains("USERNAME",Qt::CaseInsensitive))
        {
            out = account.login.toBase64()+"\r\n";
            sock->write(out);
        }
        else if(str.contains("PASSWORD",Qt::CaseInsensitive))
        {
            out = account.passwd.toBase64()+"\r\n";
            sock->write(out);
        }
        break;
    }
    qDebug() << "U: " << out;
    addLog("U:" + out);

}

QByteArray loginPlain(const Eml::Account &account)
{
    QByteArray b_arr("_" + account.login + "_" + account.passwd);
    b_arr[0] = 0;
    b_arr[account.login.size()+1] = 0;
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



}//namespace Smtp

