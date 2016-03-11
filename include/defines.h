#ifndef DEFINES_H
#define DEFINES_H
#include "QString"
#include "QByteArray"

enum M_ACCOUNT {
	YANDEX_RU,
	MAIL_RU
};

static QString _log_file_name = "Eml.log";


QByteArray str =
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

QByteArray str1 = "Новый текст \r\n"
"The default matching algorithm looks at both the file name "
"and the file contents, if necessary.<br> The file extension has "
"priority over the contents, but the contents will be used if "
"the file extension is unknown, or matches multiple MIME types.<br> "
"If fileInfo is a Unix symbolic link, the file that it refers to will "
"be used instead. If the file doesn't match any known pattern or data, "
"the default MIME type (application/octet-stream) is returned.";


QString attachs[] = { "D:/Android/Android Studio/plugins/svn4idea/lib/COPYING"
, "D:/Graphium/prj/Beyond/bin/data/gui/buttons/container_close_active.png"
, "C:/Users/Victor/Documents/Scanned Documents/коля_фото_садик.jpg"
, "C:/Users/Victor/Documents/dodatok_8_1.pdf"
, "C:/Users/Victor/Documents/icm.odt"
, "C:/Users/Victor/Documents/TeamViewer_Setup_ru.exe"
};

#endif //DEFINES_H