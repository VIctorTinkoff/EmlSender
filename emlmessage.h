#ifndef EMLMESSAGE_H
#define EMLMESSAGE_H

#include <QObject>
#include "eml.h"
#include <QRegularExpression>
#include <QVector>

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
    struct EmlPart{
        EmlPart();
        QString partId;
        QString content_type;
        QString name;
        QString content_transfer_encoding;
        QString content_disposition;
        QString file_name;

    };


public:
    explicit EmlMessage(QObject *parent = 0);
    QString  *createMessage(const QString  &body);

    void set_from(const QByteArray &from_str);
    void add_to(const QByteArray &to_str);
    void add_cc(const QByteArray &cc_str);
    void add_bcc(const QByteArray &bcc_str);
    void set_subject(const QString &subject);
    const QByteArray &from();
    const QByteArray &to();

    void attach(const QString &attach);

    const QString & mesage();

private:
    QString m_message;

    QByteArray m_sender;

    QList<QByteArray> m_to;
    QList<QByteArray> m_cc;
    QList<QByteArray> m_bcc;

    QString m_subject;
    QString m_body;

    QList<QString> m_attach;

    EmlEncodeType m_hEnc;

protected:
    QString eml_enc(const QString &str);




signals:

public slots:
};

#endif // EMLMESSAGE_H
