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
    QString  *createMessage(const QString  & sender);

    void add_to(const QString &to_str);
    void add_cc(const QString &cc_str);
    void add_bcc(const QString &bcc_str);
    void set_subject(const QString &subject);


private:
    QString m_message;
    QString m_sender;
    QString m_to;
    QString m_cc;
    QString m_bcc;
    QString m_subject;

    EmlEncodeType m_hEnc;

protected:
    QString eml_enc(const QString &str);




signals:

public slots:
};

#endif // EMLMESSAGE_H
