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
	ST_NONE			= 0x000,
	ST_HELLO		= 0x001,
	ST_AUTH			= 0x002,
	ST_LOGIN		= 0x004,
	ST_LOGIN_OK		= 0x008,
	ST_FROM			= 0x010,
	ST_FROM_OK		= 0x020,
	ST_TO			= 0x040, 
	ST_TO_OK		= 0x080,
	ST_DATA			= 0x080,
	ST_DATA_OK		= 0x100,
	ST_END			= 0x200,
	ST_ERROR		= 0x400,
	ST_STRT_DAT		= 0x800

//     ST_NONE     = 0,// 0b0000000000000,
//     ST_HELLO    = 1,//0b0000000000001,
//     ST_AUTH     = 4,//0b0000000000010,
//     ST_LOGIN    = 8,//0b0000000000100,
//     ST_LOGIN_OK = 16,//0b0000000001000,
//     ST_FROM     = 32,//0b0000000010000,
//     ST_FROM_OK  = 64,//b0000000100000,
//     ST_TO       = 128,//0b0000001000000,
//     ST_TO_OK    = 256,//0b0000010000000,
//     ST_DATA     = 512,//0b0000100000000,
//     ST_DATA_OK  = 1024,//0b0001000000000,
//     ST_END      = 2048,//0b0010000000000,
//     ST_ERROR    = 4096,//0b0100000000000,
//     ST_STRT_DAT = 8192//b1000000000000,
};

void hello(QSharedPointer<Eml::Account> account, QSslSocket *sock);

bool checkAuthMethod(const QString &ansver);

Smtp::Auth_Method authMethod(const QString & str);
Smtp::Auth_Method authMethod();

Smtp::State decodeAnsver(const QString & ansv, State last_State = ST_NONE);

QByteArray findedAuthMethod();

void login(QSharedPointer<Eml::Account> account, QSslSocket *sock, const QString & ansv );
QByteArray loginPlain(QSharedPointer<Eml::Account> account );

void addLog(const QString & log);

}//namespace Smtp

#endif // SMTP

