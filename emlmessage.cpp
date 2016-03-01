#include "emlmessage.h"
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QDateTime>
#include <QTimeZone>
#include <QVector>
#include <QUuid>
#include <QMimeData>
#include <QMimeType>
#include <QMimeDatabase>


#include "eml.h"


EmlMessage::EmlMessage(QObject *parent)
    : QObject(parent)
    , exp_is_no_latin_char(("[А-Яа-я]"))
    , encode_out_format("=?UTF-8?%1?%2?= %3")
    , exp_enc_search_addres("^(.*)(<.*>)$")
{
    m_hEnc = B_ENC;

}

QString *EmlMessage::createMessage(const QString &body)
{
    m_body = body;
    Eml::Header h(m_subject);
    m_message.append(h.header);

//      Eml::Part p(m_body);
      Eml::MultiPart p(m_body);

      m_message.append(p.part);

    for (int i = 0; i < m_attach.count(); ++i)
    {
        p.attach(m_attach.at(i));
        m_message.append(p.part);
    }




    QFile f("test.eml");
    if(f.open(QIODevice::WriteOnly))
    {
        QTextStream f_out(&f);
        f_out << m_message;
        f.close();
    }

    return &m_message;
}

void EmlMessage::set_from(const QByteArray &from_str)
{
    m_sender = from_str;
}


void EmlMessage::add_to(const QByteArray &to_str)
{
    m_to.push_back(to_str);
}

void EmlMessage::set_subject(const QString &subject)
{
    m_subject = subject;
}

const QByteArray &EmlMessage::from()
{
    return m_sender;
}

const QByteArray &EmlMessage::to()
{
    return m_to.at(0);
}

void EmlMessage::attach(const QString &attach)
{
    m_attach.push_back(attach);
}

const QString &EmlMessage::mesage()
{
    return m_message;
}



