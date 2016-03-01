#ifndef EMLSSLSOCKET_H
#define EMLSSLSOCKET_H

#include <QObject>
#include <QHostAddress>
#include "eml.h"


//class QHostAddress;
class QAbstractSocket;
class QCryptographicHash;
class QSslSocket;
class QCryptographicHash;
class QSslError;

class EmlSSlSocket : public QObject
{
    Q_OBJECT
public:
    explicit EmlSSlSocket(QObject *parent = 0);
    virtual ~EmlSSlSocket();
    void setAccount(const Eml::Account & account );
    void setPort(int port);
    const QString &lastError();
    QByteArray loginPlain();
public:

signals:
    void signalError(const QString&);

private:
    QSslSocket * m_p_socket;
    Eml::Account m_account;

    int m_nport;
    QString m_lastError;
public slots:
     void sslConnect();
private slots:
    void updateEnabledStatate();

    void socketStateChanged(QAbstractSocket::SocketState st);
    void socketEncripted();
    void socketReadyRead();
    void socketSendData(const QString &message);
    void socketError(QAbstractSocket::SocketError err);
    void sslError(const QList<QSslError> &err_s);
    void slotError(const QString & err);
};

#endif // EMLSSLSOCKET_H
