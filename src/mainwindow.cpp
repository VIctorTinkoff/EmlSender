#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStatusBar>
#include <QSettings>
#include <QComboBox>
#include "taskform.h"



extern QSettings sett;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_p_form = new TaskForm;

    setCentralWidget(m_p_form);
    m_p_message = 0;


    m_p_socket = new SmtpSSlSocket(this);
    loadSetting();

    connect(m_p_socket,SIGNAL(sendRecipientsTo()),this,SLOT(slotSendRecipientTo()));
    connect(m_p_socket,SIGNAL(smtpSendData()),this,SLOT(slotSendSmtpData()));

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

void MainWindow::createMessage(EmlMessage *message)
{

}



void MainWindow::on_actionCnnect_smtp_triggered()
{
    m_p_socket->smtp_connect(m_p_form->account());
}





void MainWindow::slotSendRecipientTo()
{
    static Task * task = m_p_form->tasks().front();
    static QStringList * addresList = &task->addressList;

    static int id = addresList->count();

    if (id < 0)
    {
        qWarning("slotSendRecipoentTo : End data from addresList" );
        return;
    }

    if(--id >= 0)
    {
        qDebug() << "+++++ send recipient to ++++++++ "<<id << ":" << addresList->at(id) ;
        m_p_socket->slotSendRecipient(addresList->at(id));
    }
    else
    {
        m_p_socket->slotSendRecipient();
        qDebug() << "+++++ send recipient to ++++++++ DATA " ;
    }

}

void MainWindow::slotSendSmtpData()
{
    QString tmp;

    m_p_message = new EmlMessage;
    m_p_message->out_stream.reset(new QTextStream(&tmp,QIODevice::ReadWrite));
    m_p_message->header().data()->set_from(m_p_form->account()->login.toUtf8());

    Task * task = m_p_form->tasks().first();
    for (int i = 0; i < task->addressList.count(); ++i)
    {
        m_p_message->header().data()->append_to(task->addressList.at(i).toUtf8());
    }

    task = 0;

    m_p_message->header().data()->set_subject("Hello this is test");


    m_p_message->createMessage("Hello");


    ui->textEdit->append(tmp);

    delete m_p_message;

    m_p_socket->slotSmtpSendData(tmp.toLocal8Bit(),true);


    qDebug() << "++++++ send Data ++++++++";
}

void MainWindow::on_pushButton_clicked()
{
    slotSendSmtpData();
}
