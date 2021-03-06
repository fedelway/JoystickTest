#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <../JoystickServer/joystickbuttons.h>
#include <../JoystickServer/programprotocol.h>
#include <QTcpSocket>
#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnStart_clicked()
{
    if(socket.state() == QAbstractSocket::ConnectedState)
    {
        socket.close();
        ui->btnStart->setText("Start");
    }else{
        const QString address = "127.0.0.1";
        const quint16 port = 1060;

        socket.setSocketOption(QAbstractSocket::LowDelayOption,1);
        socket.connectToHost(address,port);

        ui->btnStart->setText("Stop");
    }
}

void MainWindow::sendData(int button, bool press)
{
    const bool newProtocol = true;

    if(newProtocol){
        char data[9];

        sprintf(data,"MSG004%02d%s",button,press?"S":"N");

        socket.write(data,9);
    }else{
        const int packetSize = Protocol::headerSize() + Protocol::dataSize();

        //Armo el packete
        Protocol::header_t header;
        Protocol::data_t data;

        header.size = Protocol::dataSize();
        strcpy((char*)&header.validationString,(char*)Protocol::ValidationString);
        data.btn = button;
        data.press = press ? 1 : 0;

        char* buffer = (char*)malloc(packetSize);

        memcpy(buffer,&header,Protocol::headerSize());
        memcpy(buffer+Protocol::headerSize(),&data,Protocol::dataSize());

        socket.write(buffer,packetSize);

        free(buffer);
    }
}

void MainWindow::on_btnUp_pressed()
{
    const int btn = Joystick::BTN_UP;
    sendData(btn,true);
}

void MainWindow::on_btnDown_pressed()
{
    const int btn = Joystick::BTN_DOWN;
    sendData(btn,true);
}

void MainWindow::on_btnLeft_pressed()
{
    const int btn = Joystick::BTN_LEFT;
    sendData(btn,true);
}

void MainWindow::on_btnRight_pressed()
{
    const int btn = Joystick::BTN_RIGHT;
    sendData(btn,true);
}

void MainWindow::on_btnUp_released()
{
    const int btn = Joystick::BTN_UP;
    sendData(btn,false);
}

void MainWindow::on_btnDown_released()
{
    const int btn = Joystick::BTN_DOWN;
    sendData(btn,false);
}

void MainWindow::on_btnLeft_released()
{
    const int btn = Joystick::BTN_LEFT;
    sendData(btn,false);
}

void MainWindow::on_btnRight_released()
{
    const int btn = Joystick::BTN_RIGHT;
    sendData(btn,false);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << QKeySequence(keyEvent->key()).toString()
                 << ((event->type() == QEvent::KeyPress) ? "PRESS" : "RELEASE");
        ui->lineEdit->setText(ui->lineEdit->text().append(QKeySequence(keyEvent->key()).toString()));
    }

    return QMainWindow::eventFilter(watched,event);
}
