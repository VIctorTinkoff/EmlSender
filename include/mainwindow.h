#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslSocket>
#include "eml.h"
#include "recipientmodel.h"
#include "emlsocket.h"


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
    EmlSocket * m_p_socket;
	QVector< Eml::Account *> m_accounts;
	QList< Eml::Recipient *> m_recipients;
	RecipientModel *m_model;

private:
	void saveSetting();
	void loadSetting();


public slots:
	void connect_smtp(const QString &hostName, int nPort);
	void on_accountAddButton_clicked();
	void on_actionCnnect_smtp_triggered();
	void on_accountCBox_currentIndexChanged(int index);
	void on_editAccountButton_clicked();
	void on_taskDeleteButton_clicked();
	void on_taskInsertButton_clicked();


private slots :
	void updateEnabledStatate();
	void socketStateChanged(QAbstractSocket::SocketState st);
	void socketEncripted();
	void socketReadyRead();
	void socketSendData(const QString &message);
	void socketError(QAbstractSocket::SocketError err);
	void sslError(const QList<QSslError> &err_s);
	void slotError(const QString & err);
};

#endif // MAINWINDOW_H
