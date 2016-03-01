#ifndef EML_H
#define EML_H
#include <QObject>
#include <QFile>

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

struct Account {
    Account();
    Account(const QByteArray & p,const QByteArray & l,const QString & h );
    Account &operator =(const Account &a);
    QByteArray passwd;
    QByteArray login;
    QString hostName;
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

int port(Eml::Ports p);



}//namespace Eml


#endif // EML_H
