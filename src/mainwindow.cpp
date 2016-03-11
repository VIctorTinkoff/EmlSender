#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include <QDebug>
#include <QStatusBar>
#include <QSettings>
#include <QComboBox>


extern QSettings sett;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_p_socket = new EmlSocket(this);
	
	loadSetting();
	
}

MainWindow::~MainWindow()
{

	saveSetting();

	delete ui;
    delete m_p_socket;
}

void MainWindow::saveSetting()
{

}

void MainWindow::loadSetting()
{
}

void MainWindow::connect_smtp(const QString &hostName, int nPort)
{
	if (!m_p_socket)
	{
//        m_p_socket = new QSslSocket(this);

//        connect(m_p_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
//            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
//        connect(m_p_socket, SIGNAL(encrypted()),
//            this, SLOT(socketEncripted()));
//        connect(m_p_socket, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(socketError(QAbstractSocket::SocketError)));
//        connect(m_p_socket, SIGNAL(sslErrors(QList<QSslError>)),
//            this, SLOT(sslError(QList<QSslError>)));
//        connect(m_p_socket, SIGNAL(readyRead()),
//            this, SLOT(socketReadyRead()));
	}

//	m_p_socket->connectToHostEncrypted(hostName, nPort);

}

void MainWindow::on_accountAddButton_clicked()
{
}

void MainWindow::on_actionCnnect_smtp_triggered()
{
}

void MainWindow::on_accountCBox_currentIndexChanged(int index)
{
}

void MainWindow::on_editAccountButton_clicked()
{
}

void MainWindow::on_taskDeleteButton_clicked()
{
}

void MainWindow::on_taskInsertButton_clicked()
{
}

void MainWindow::updateEnabledStatate()
{
	qDebug() << "==== updateEnabledStatate";
}

void MainWindow::socketStateChanged(QAbstractSocket::SocketState st)
{
	qDebug() << "=== socketStateChanged";

}

void MainWindow::socketEncripted()
{
	qDebug() << "==== socketEncripted";
}

void MainWindow::socketReadyRead()
{
	qDebug() << "==== socketReadyRead";

}

void MainWindow::socketSendData(const QString &message)
{
	qDebug() << "==== socketSendData";
}

void MainWindow::socketError(QAbstractSocket::SocketError err)
{
	qDebug() << "==== socketError";
}

void MainWindow::sslError(const QList<QSslError> &err_s)
{
	qDebug() << "==== sslError";
}

void MainWindow::slotError(const QString & err)
{
	qDebug() << "==== slotError";
}
