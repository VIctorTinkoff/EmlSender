/*
*   To create an e-mail is necessary to determine
*
* sample:
*   EmlMessage message
*   message.header().data()->set_from(sender <email@addres>);
*   message.header().data()->set_subject(<text subject emaill>);
*   message.header().data()->append_to(recipient-1<email@adress>);
*   ...
*   message.header().data()->append_to(recipient-n<email@adress>);
*   message.data()->setBody(<email body>);
*
*   QString f; - Here is the generated message
*
*   QFile f(<fileName.eml>)--For output to a file
*
*   QTextStream s_out(&f,QIODevice::ReadWrite);
*   m_p_message.out_stream = s_out;
*   m_p_message.createMessage();
*
*/


#ifndef EMLMESSAGE_H
#define EMLMESSAGE_H

#include <QObject>
#include "emlheader.h"
#include <QRegularExpression>
#include <QVector>
#include <QSharedPointer>
#include <QTextStream>

class EmlMessage : public QObject
{
    Q_OBJECT
    enum EmlEncodeType{
        B_ENC,
        Q_ENC
    };

    const QRegExp exp_is_no_latin_char;
    const QString encode_out_format;
    const QRegularExpression exp_enc_search_addres;

public:
    explicit EmlMessage(QTextStream *s_out = 0, QObject *parent = 0);
    virtual ~EmlMessage();
    void createMessage(const QByteArray &body);
    void createMessage();
    QSharedPointer<Eml::Header> header() const;
    void attach(const QString &attach);
    void setBody(const QByteArray & body);

    QSharedPointer<QTextStream> out_stream;

private:
    QSharedPointer<Eml::Header> m_eml_heder;
    QByteArray m_body;
    QVector<QString> m_attach;

    EmlEncodeType m_hEnc;

signals:

public slots:
};

#endif // EMLMESSAGE_H
