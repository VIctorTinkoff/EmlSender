#ifndef EML_H
#define EML_H
#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QStringList>
#include <QVector>


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
	Account(const QByteArray & p, const QByteArray & l, const QString & h, int port = 0);
	~Account();
	Account &operator =(const Account &a);
	void fromByteArray(const QByteArray & ba);
	QByteArray toByteArray();

	QString itemString(bool hostName = true, bool nport = true, bool login = false, const QString & sep = ":");
	QByteArray passwd;
	QString hostName;
	QByteArray login;
	int n_port;

};

struct Recipient {
	Recipient();
	void fromByteArray(const QByteArray & ba);
	QByteArray toByteArray();
	Recipient &operator =(const Recipient &r);
	QString title;
	QList<QString> adresses;
	QString path;
	QVector<int> objIdList;
	

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


QString eml_date();
QString eml_createUniqueBoundId(const QString &name = "PART");
QString eml_create_tag(const QString &str_val);
QString eml_create_tag(const QVector<QByteArray> &va_list);
/*
 * 'B' - encoding is identical to the "BASE64" encoding defined by RFC2045.
 * 'Q' - encoding is similar to the "Quoted-Printable" content-transfer-encoding defined in RFC 2045.
 */
QString enc(const QString &str, QChar coding='B');


int port(Eml::Ports p);



}//namespace Eml


#endif // EML_H
