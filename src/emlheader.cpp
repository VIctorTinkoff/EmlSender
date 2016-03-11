#include "emlheader.h"
#include "eml.h"
#include <QVector>

namespace  Eml {


Header::Header()
{
}

Header::Header(const QString &subject)
{
    m_subject = subject;
//    header.append(QString("Subject: %1\r\n").arg(eml_create_tag(subject)));
//    header.append("MIME-Version: 1.0 \r\n");
}

Header::Header(const QString &from, const QVector<QByteArray> &to, const QString &subject, const QString &cc, const QString &bcc)
{
    m_from = from.toUtf8();
    m_to = to;
    m_subject = subject;
    if(!cc.isEmpty())
        m_cc.append(cc.toUtf8());
    if(!bcc.isEmpty())
        m_bcc.append(bcc.toUtf8());
}

QByteArray Header::header()
{
    QByteArray header;
    header.append(eml_date() +  "\r\n");
    header.append("From: " + eml_create_tag(m_from) + "\r\n");

    header.append(QString("To: %1\r\n").arg(eml_create_tag(m_to)));

    header.append(QString("Subject: %1\r\n").arg(eml_create_tag(m_subject)));

    if(!m_cc.isEmpty())
        header.append(QString("Cc: %1\r\n").arg(eml_create_tag(m_cc)));
    if(!m_bcc.isEmpty())
        header.append(QString("Bcc: %1\r\n").arg(eml_create_tag(m_bcc)));

    header.append("MIME-Version: 1.0 \r\n");

    return header;
}

void Header::set_from(const QByteArray &str)
{
    m_from = str;
}

void Header::set_subject(const QString &str)
{
    m_subject = str;
}

void Header::set_to(const QVector<QByteArray> &list)
{
    m_to = list;
}

void Header::append_to(const QByteArray &str)
{
    m_to.append(str);
}

void Header::append_cc(const QByteArray &str)
{
    m_cc.append(str);
}

void Header::append_bcc(const QByteArray &str)
{
    m_bcc.append(str);
}

bool Header::isValid()
{
    if(m_from.isEmpty() || m_to.isEmpty())
    {
        return false;
    }

    return true;
}

const QByteArray &Header::from()
{
    return m_from;
}

const QVector<QByteArray> &Header::to()
{
    return m_to;
}



}//namespace  Eml
