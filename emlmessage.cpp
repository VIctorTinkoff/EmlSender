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

QString *EmlMessage::createMessage(const QString &sender)
{
    m_sender = sender;

    QString str =
            "<html>\r\n"
            "  <head>\r\n"
            "    <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\r\n"
            "    <title>Hello </title>\r\n"
            "  </head>\r\n"
            "  <body text=\"#FF0000\" bgcolor=\"#FFFFFF\">\r\n"
            "   <h2><center> This is boody.</center></h2>\r\n"
            "  <body text=\"#00FF00\" bgcolor=\"#FFFFFF\">\r\n"
            "   <h1><center> Просто текст</center></h1>\r\n"
            "  <body text=\"#000000\" bgcolor=\"#99FF99\">\r\n"
            "   <h3><center> Текст на русском языке.</center></h3>\r\n"
            "The default matching algorithm looks at both the file name "
            "and the file contents, if necessary.<br> The file extension has "
            "priority over the contents, but the contents will be used if "
            "the file extension is unknown, or matches multiple MIME types.<br> "
            "If fileInfo is a Unix symbolic link, the file that it refers to will "
            "be used instead. If the file doesn't match any known pattern or data, "
            "the default MIME type (application/octet-stream) is returned."
            "  </body>\r\n"
            "</html>\r\n";

    QString str1 = "Новый текст \r\n"
            "The default matching algorithm looks at both the file name "
            "and the file contents, if necessary.<br> The file extension has "
            "priority over the contents, but the contents will be used if "
            "the file extension is unknown, or matches multiple MIME types.<br> "
            "If fileInfo is a Unix symbolic link, the file that it refers to will "
            "be used instead. If the file doesn't match any known pattern or data, "
            "the default MIME type (application/octet-stream) is returned.";



    Eml::Header h(m_sender, m_to, m_subject);

    m_message.append(h.header);

    //Eml::Part p(str1);
    Eml::MultiPart p(str1);
//    p.attach("D:/Android/Android Studio/plugins/svn4idea/lib/COPYING");
//    p.attach("D:/Graphium/prj/Beyond/bin/data/gui/buttons/container_close_active.png");
//    p.attach("C:/Users/Victor/Documents/Scanned Documents/коля_фото_садик.jpg");
//    p.attach("C:/Users/Victor/Documents/dodatok_8_1.pdf");
//    p.attach("C:/Users/Victor/Documents/icm.odt");
//    p.attach("C:/Users/Victor/Documents/TeamViewer_Setup_ru.exe");


    m_message.append(p.part);


    QFile f("test.eml");
    if(f.open(QIODevice::WriteOnly))
    {
        QTextStream f_out(&f);
        f_out << m_message;
        f.close();
    }

    return &m_message;
}

void EmlMessage::add_to(const QString &to_str)
{
    if(m_to.isEmpty())
    {
        m_to.append(to_str);
    }
    else
    {
        m_to.append("," + to_str) ;
    }

}

void EmlMessage::set_subject(const QString &subject)
{
    m_subject = subject;
}



