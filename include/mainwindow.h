#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslSocket>
#include "eml.h"
#include "recipientmodel.h"
#include "smtpsslsocket.h"
#include "taskform.h"
#include "emlmessage.h"

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
	

private:
    Ui::MainWindow *ui;
    SmtpSSlSocket * m_p_socket;
    TaskForm * m_p_form;
    EmlMessage * m_p_message;


private:
	void saveSetting();
	void loadSetting();
    void createMessage(EmlMessage * message);



public slots:
    void on_actionCnnect_smtp_triggered();

private slots :

    void slotSendRecipientTo();
    void slotSendSmtpData();

    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
