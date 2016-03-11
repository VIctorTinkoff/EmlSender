#include <QUuid>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDateTime>
#include <QTimeZone>
#include <QMimeDatabase>
#include <QFileInfo>
#include "eml.h"
#include <QDebug>
#include <QByteArray>
#include <QDataStream>



namespace Eml
{
const char * week_str[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Su"};
const char * month_str[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const int ports[] = {25, 465, 110, 995, 143, 993, 119, 563, 0};


QString eml_createUniqueBoundId(const QString &name )
{
    static int id = 0;

    return QString("%1%2%3")
            .arg(name)
            .arg(QString(QUuid::createUuid().toByteArray())
                 .replace(QRegularExpression("[{}]"),"-")
                 )
            .arg(QString::asprintf("%04d",++id));

}

QString eml_date()
{
    int shift_utc =  QDateTime::currentDateTime()
            .timeZone()
            .standardTimeOffset(QDateTime()) / 60 / 60;

    QDateTime utc_dt = QDateTime::currentDateTimeUtc();

    return QString("Date: %1, %2 %3 %4 %5:%6:%7 %8")
            .arg(week_str[utc_dt.date().dayOfWeek() - 1])
            .arg(utc_dt.date().day())
            .arg(month_str[utc_dt.date().month()-1])
            .arg(utc_dt.date().year())
            .arg(utc_dt.time().hour())
            .arg(utc_dt.time().minute())
            .arg(utc_dt.time().second())
            .arg(QString::asprintf("+%04d",shift_utc)
                 );
}

QString eml_create_tag(const QString &str_val)
{

    QStringList list = str_val.split(",");
    QString result;

    for (int i = 0; i < list.count(); ++i)
    {
        if(list.at(i).contains(QRegularExpression("[А-Яа-я]")))
        {
            result.append(enc(list.at(i)));
        }
        else
        {
            result.append(list.at(i));
        }
        if (list.count() > 1 && i < list.count()-1)
            result.append(",\r\n\t");
    }
    return result;
}

QString eml_create_tag(const QVector<QByteArray> &va_list)
{
    QString result = "";
    for (int i = 0; i < va_list.count(); ++i)
    {
        result.append(eml_create_tag(va_list.at(i)));
        if (i < va_list.count() - 1)
            result.append("\r\n\t");
    }
    return result;
}



QString enc(const QString &str, QChar coding)
{

    QRegularExpressionMatch math_addres = QRegularExpression("^(.*)(<.*>)$").match(str);
    QString alias = "";
    QString addres = "";
    QString out_str = "";

    if (math_addres.hasMatch())
    {
        alias = math_addres.captured(1);
        addres = math_addres.captured(2);
    }
    else
    {
        alias = str;
    }

    if (coding == 'Q')
        out_str = alias.toUtf8().toPercentEncoding("{}","{}",'=');
    else if (coding == 'B')
        out_str = alias.toUtf8().toBase64();

    return QString("=?UTF-8?%1?%2?= %3")
            .arg(coding)
            .arg(out_str)
            .arg(addres)
            ;
}



Part::Part(const QString &body)
{
    QMimeDatabase mdb;

    part.append(QString("Content-Type: %1; charset=utf-8\r\n")
                .arg(mdb.mimeTypeForData(body.toUtf8()).name()));

    if(body.contains(QRegularExpression("[А-Яа-я]")))
    {
        part.append("Content-Transfer-Encoding: base64");
        part.append("\r\n\r\n");

        QByteArray arr = body.toUtf8().toBase64();
        int line_cout = 0;
        QString str;

        for (int i = 0; i < arr.size(); ++i )
        {
            if (line_cout < 78)
            {
                str.append(arr.at(i));
            }
            else
            {
                str.append("\r\n");
                line_cout = 0;
                str.append(arr.at(i));
            }
            line_cout++;
        }
        part.append(str);
    }
    else
    {
        part.append("\r\n");
        part.append(body);
        part.append("\r\n");
    }
}

MultiPart::MultiPart(const QString &body)
{
    part_id = eml_createUniqueBoundId();
    Part p(body);

    part.append("Content-Type: multipart/mixed;\r\n\t");
    part.append(QString("boundary=\"%1\"").arg(part_id));
    part.append("\r\n\r\n\r\n");

    part.append(QString("--%1\r\n").arg(part_id));
    part.append(QString("%1\r\n").arg(p.part));
    part.append(QString("--%1\r\n").arg(part_id));
}

void MultiPart::attach(const QString &file_name)
{
    QFile f(file_name);
    QFileInfo f_info(f);
    QMimeDatabase mdb;

    qDebug() << f_info.fileName() << ":" << mdb.mimeTypeForFile(f_info).name();

    if(!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "error open file " << file_name << f.errorString();
        return;
    }

    part.append(QString("Content-Disposition: attachment;\r\n\t filename=\"%1\"\r\n")
                .arg(eml_create_tag(f_info.fileName()))
                );

    part.append("MIME-Version: 1.0 \r\n");
    part.append("Content-Transfer-Encoding: base64\r\n");

    part.append(QString("Content-Type: %1;\r\n\t name=\"%2\"\r\n")
                .arg(mdb.mimeTypeForFile(f_info).name())
                .arg(eml_create_tag(f_info.fileName()))
                );
    part.append("\r\n");

    //---------------------- attach file to message----------------------
    QByteArray barr(f.readAll().toBase64());
    int len_str = 0;

    for (int i = 0; i < barr.size(); ++i)
    {
        if(len_str > 78)
        {
            part.append("\r\n");
            len_str = 0;
        }
        part.append(barr.at(i));
        len_str++;
    }
    f.close();
    //--------------------------------------------------------------------

    part.append("\r\n");
    part.append(QString("--%1\r\n").arg(part_id));

}

int port(Ports p)
{
    return ports[p];
}

Account::Account()
{
    passwd = "";
    login = "";
    hostName = "LocalHost";

}

Account::Account(const QByteArray & p, const QByteArray & l, const QString & h, int port /*= 0*/)
{
	passwd = p;
	login = l;
	hostName = h;
	n_port = port;

}

Account::~Account()
{

}

void Account::fromByteArray(const QByteArray & ba)
{
	QDataStream ba_in(ba);
	ba_in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	ba_in >> passwd >> login >> hostName >> n_port;
}

QByteArray Account::toByteArray()
{
	QByteArray ba;
	QDataStream ba_out(&ba, QIODevice::WriteOnly);
	ba_out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	ba_out << passwd << login << hostName << n_port;

	return ba;
}

QString Account::itemString(bool bhostName/*=true*/, bool bnport/*=true*/, bool blogin/*=false*/, const QString & sep/*=':'*/)
{
	QString out_str = QString("%1%2%3%4%5").arg(bhostName ? hostName : "")
					.arg(bhostName ? sep : "")
					.arg(blogin ? QString(login) : "")
					.arg(blogin ? sep : "")
					.arg(bnport ? QString("%1").arg(this->n_port) : "");


	return out_str;
}

Account &Account::operator =(const Account &a)
{
    passwd = a.passwd;
    login = a.login;
    hostName = a.hostName;
	this->n_port = a.n_port;

    return *this;
}



Recipient::Recipient()
{
	title = QObject::tr("<undefined>");
	path = QObject::tr("<undefined>");
}

void Recipient::fromByteArray(const QByteArray & ba)
{
	QDataStream ba_in(ba);
	ba_in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	int ba_size;
	ba_in >> title >> path >> ba_size;

	for (int i = 0; ba_size; ++i)
	{
		ba_in << adresses[i];
	}
	int obj_size;
	ba_in >> obj_size;

	for (int i = 0; i < obj_size; ++i)
	{
		ba_in >> objIdList[i];
	}

}

QByteArray Recipient::toByteArray()
{
	QByteArray ba;
	QDataStream ba_out(&ba,QIODevice::WriteOnly);
	ba_out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	
	ba_out << title << path << adresses.size();
	for (int i = 0; adresses.size(); ++i)
	{
		ba_out << adresses[i];
	}
	ba_out << objIdList.size();
	
	for (int i = 0; i < objIdList.size(); ++i)
	{
		ba_out << objIdList[i];
	}

	return ba;
}

Recipient & Recipient::operator=(const Recipient &r)
{
	adresses = r.adresses;
	objIdList = r.objIdList;
	path = r.path;
	title = r.title;

	return *this;
}

}//namespace Eml
