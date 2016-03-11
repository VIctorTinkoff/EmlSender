
#ifndef EMLSSLSOCKET_H
#define EMLSSLSOCKET_H

#include <QObject>
#include <QQueue>
#include <QHostAddress>
#include <QPointer>
#include "emlmessage.h"
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
    explicit EmlSSlSocket(EmlMessage * mes = 0, QObject *parent = 0);
    virtual ~EmlSSlSocket();
    void setAccount(Eml::Account * account );
    void setPort(int port);
    const QString &lastError();
    QSslSocket * socket();
    QByteArray loginPlain();

	int sendMessage(EmlMessage * mess);
	int writeSocket(const QByteArray & b_arr);

public:

signals:
    void signalError(const QString&);

private:
    QSslSocket * m_p_socket;
    EmlMessage * m_p_message;

	QSharedPointer<Eml::Account> m_p_account;
    QVector<QByteArray> m_recipiens;

    QString * m_p_buffer;

    QSharedPointer<QTextStream> m_p_stream;
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
