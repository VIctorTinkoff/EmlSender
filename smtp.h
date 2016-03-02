#ifndef SMTP
#define SMTP
#include <QObject>
#include "eml.h"

class QSslSocket;
class QString;
//class Eml::Account;
namespace Smtp {

//enum Auth_Method {
//    PLAIN,
//    LOGIN,
//    COUNT_METHOD,
//    NO_FIND_METHOD
//};

enum Auth_Method {
    LOGIN,
    PLAIN,
    COUNT_METHOD,
    NO_FIND_METHOD
};

enum State{
    ST_HELLO,
    ST_AUTH,
    ST_LOGIN,
    ST_LOGIN_OK,
    ST_LOGIN_ERR,
    ST_FROM,
    ST_FROM_OK,
    ST_FROM_ERR,
    ST_TO,
    ST_TO_OK,
    ST_TO_ERR,
    ST_DATA,
    ST_DATA_OK,
    ST_DATA_ERR,
    ST_DATA_END,
    ST_END,
    ST_ERROR
};

void hello(const Eml::Account & account, QSslSocket *sock);
bool checkAuthMethod(const QString &ansver);

Smtp::Auth_Method authMethod(const QString & str);
Smtp::Auth_Method authMethod();

Smtp::State decodeAnsver(const QString & ansv);

QByteArray findedAuthMethod();

void login(const Eml::Account &account, QSslSocket *sock, const QString & ansv );
QByteArray loginPlain(const Eml::Account & account );

void addLog(const QString & log);

}//namespace Smtp

#endif // SMTP

