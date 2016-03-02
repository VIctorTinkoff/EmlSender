#ifndef EML_H
#define EML_H
#include <QObject>
#include <QFile>
#include <QByteArray>

namespace Eml {

enum Ports{
    SMTP_,
    S_SMTP,
    POP3,
    S_POP3,
    IMAP,
    S_IMAP,
    NNTP,
    S_NNTP,
    NO_PORT
};


//QString eml_date();
QString eml_createUniqueBoundId(const QString &name = "PART");
QString eml_create_tag(const QString &str_val);
/*
 * 'B' - encoding is identical to the "BASE64" encoding defined by RFC2045.
 * 'Q' - encoding is similar to the "Quoted-Printable" content-transfer-encoding defined in RFC 2045.
 */
QString enc(const QString &str, QChar coding='B');



struct Header{
    Header(const QString &subject);
    Header(const QString &from, const QVector<QByteArray> &to, const QString &subject, const QString &cc="", const QString &bcc="");
    QString header;
};

struct Part{
    Part(const QString &body);
    QString part;
};

struct MultiPart{
    MultiPart(const QString &body);
    void attach(const QString & file_name);

    QString part;
    QString part_id;
};

struct Account {
    Account();
    Account(const QByteArray & p,const QByteArray & l,const QString & h );
    Account &operator =(const Account &a);
    QByteArray passwd;
    QString hostName;
    QByteArray login;

};



int port(Eml::Ports p);



}//namespace Eml


#endif // EML_H
