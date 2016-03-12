#ifndef SMTPSSLSOCKET_H
#define SMTPSSLSOCKET_H

#include <QSslSocket>
#include "account.h"

class SmtpSSlSocket : public QSslSocket
{
    Q_OBJECT
public:
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
    };

public:
    explicit SmtpSSlSocket(QObject * parent);
    void smtp_connect(Account *account);

private:
    Account * m_accaunt;
    Auth_Method m_auth_method;
    State m_last_state;
private:
    bool checkAuthMethod(const QString &ansver);
    Auth_Method authMethod(const QString & str);
    State decodeAnsver(const QString & ansv);
    void login(const QString & ansv );
    //QByteArray findedAuthMethod();
signals:
    void sendRecipientsTo();
    void smtpSendData();

private slots:
    void slot_hello();
    void slot_auth(const QByteArray & ansv);
    void from();

public slots:
    void slotReadyRad();
    void slotSendRecipient(const QString &address = QString());
    void slotSmtpSendData(const QByteArray &data, bool end_data = false);
};

#endif // SMTPSSLSOCKET_H
