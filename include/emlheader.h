#ifndef EMLHEADER_H
#define EMLHEADER_H
#include <QVector>

namespace Eml {


class Header
{
public:
    Header();
    Header(const QString &subject);
    Header(const QString &from, const QVector<QByteArray> &to, const QString &subject, const QString &cc="", const QString &bcc="");

    QByteArray header();

    void set_from(const QByteArray &str);
    void set_subject(const QString & str);
    void set_to(const QVector<QByteArray> & list);
    void append_to(const QByteArray &str);
    void append_cc(const QByteArray &str);
    void append_bcc(const QByteArray &str);

    bool isValid();

    const QByteArray &from();
    const QVector<QByteArray> &to();

private:
    QByteArray m_from;
    QVector<QByteArray> m_to;
    QVector<QByteArray> m_cc;
    QVector<QByteArray> m_bcc;
    QString m_subject;
};
}//namespace Eml
#endif // EMLHEADER_H
