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

void hello(const Eml::Account & account, QSslSocket *sock);
bool checkAuthMethod(const QString &ansver);

Smtp::Auth_Method authMethod(const QString & str);
Smtp::Auth_Method authMethod();

QByteArray findedAuthMethod();

void login(const Eml::Account &account, QSslSocket *sock, const QString & ansv );
QByteArray loginPlain(const Eml::Account & account );

void addLog(const QString & log);

}//namespace Smtp

#endif // SMTP

