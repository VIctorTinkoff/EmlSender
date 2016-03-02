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
    ST_NONE     = 0b0000000000000,
    ST_HELLO    = 0b0000000000001,
    ST_AUTH     = 0b0000000000010,
    ST_LOGIN    = 0b0000000000100,
    ST_LOGIN_OK = 0b0000000001000,
    ST_FROM     = 0b0000000010000,
    ST_FROM_OK  = 0b0000000100000,
    ST_TO       = 0b0000001000000,
    ST_TO_OK    = 0b0000010000000,
    ST_DATA     = 0b0000100000000,
    ST_DATA_OK  = 0b0001000000000,
    ST_END      = 0b0010000000000,
    ST_ERROR    = 0b0100000000000,
    ST_STRT_DAT = 0b1000000000000,
};

void hello(const Eml::Account & account, QSslSocket *sock);
bool checkAuthMethod(const QString &ansver);

Smtp::Auth_Method authMethod(const QString & str);
Smtp::Auth_Method authMethod();

Smtp::State decodeAnsver(const QString & ansv, State last_State = ST_NONE);

QByteArray findedAuthMethod();

void login(const Eml::Account &account, QSslSocket *sock, const QString & ansv );
QByteArray loginPlain(const Eml::Account & account );

void addLog(const QString & log);

}//namespace Smtp

#endif // SMTP

