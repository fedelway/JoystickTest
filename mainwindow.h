#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_btnStart_clicked();

    void on_btnLeft_pressed();

    void on_btnUp_pressed();

    void on_btnRight_pressed();

    void on_btnDown_pressed();

    void on_btnUp_released();

    void on_btnDown_released();

    void on_btnLeft_released();

    void on_btnRight_released();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;

    void sendData(int btn, bool press);
};

#endif // MAINWINDOW_H
