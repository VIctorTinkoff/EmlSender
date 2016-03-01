#ifndef EMLSOCKET_H
#define EMLSOCKET_H

#include <QObject>

class EmlSocket : public QObject
{
    Q_OBJECT
public:
    explicit EmlSocket(QObject *parent = 0);

signals:

public slots:
};

#endif // EMLSOCKET_H