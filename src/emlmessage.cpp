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

#include "emlheader.h"
#include "eml.h"


EmlMessage::EmlMessage(QTextStream *s_out, QObject *parent)
    : QObject(parent)
    , exp_is_no_latin_char(("[А-Яа-я]"))
    , encode_out_format("=?UTF-8?%1?%2?= %3")
    , exp_enc_search_addres("^(.*)(<.*>)$")
{
    out_stream.reset(s_out);

    m_hEnc = B_ENC;

    m_eml_heder.reset(new Eml::Header());

}

EmlMessage::~EmlMessage()
{
    m_eml_heder.clear();
    out_stream.clear();
}

void EmlMessage::createMessage(const QByteArray &body)
{
    m_body = body;

    *out_stream << m_eml_heder.data()->header();

    QScopedPointer<Eml::MultiPart> part(new Eml::MultiPart(m_body));

    for (int i = 0; i < m_attach.count(); ++i)
    {
        part.data()->attach(m_attach.at(i));

    }

    *out_stream << part.data()->part;

    part.reset();

}

void EmlMessage::createMessage()
{
    createMessage(m_body);
}

QSharedPointer<Eml::Header> EmlMessage::header() const
{
    return m_eml_heder;
}


void EmlMessage::attach(const QString &attach)
{
    m_attach.push_back(attach);
}

void EmlMessage::setBody(const QByteArray &body)
{
    m_body = body;
}

//const QString &EmlMessage::mesage()
//{
//    return m_message;
//}



