#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include  <QTcpSocket>
#include <QTimer>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer *myTimer;
    void UP();
    void DOWN();
    void LEFT();
    void RIGHT();

/*---------------*/
    void STOP();
    void goHead();
    void goBack();
    void goLeft();
    void goRight();
    void goLeftHead();
    void goLeftBack();
    void goRightHead();
    void goRightBack();

private slots:
    void on_pushButtonConnect_clicked();
    void sendcmd();

private:
    Ui::MainWindow *ui;

    QTcpSocket* pictureSocket;
    QTcpSocket* controlSocket;

private slots:
    void startTime();
    void getpic();
    void tips();
    void on_pushButton_LEFT_pressed();
    void on_pushButton_LEFT_released();
    void on_pushButton_UP_pressed();
    void on_pushButton_DOWN_pressed();
    void on_pushButton_DOWN_released();
    void on_pushButton_UP_released();
    void on_pushButton_RIGHT_pressed();
    void on_pushButton_RIGHT_released();
    void on_pushButtonCARFRONT_pressed();
    void on_pushButtonCARFRONT_released();
    void on_pushButtonCARLF_pressed();
    void on_pushButtonCARLF_released();
    void on_pushButtonCARRF_pressed();
    void on_pushButtonCARRF_released();
    void on_pushButtonCARLEFT_pressed();
    void on_pushButtonCARLEFT_released();
    void on_pushButtonCARRIGHT_pressed();
    void on_pushButtonCARRIGHT_released();
    void on_pushButtonCARLB_pressed();
    void on_pushButtonCARLB_released();
    void on_pushButtonCARBACK_pressed();
    void on_pushButtonCARBACK_released();
    void on_pushButtonCARRB_pressed();
    void on_pushButtonCARRB_released();
    void on_pushButtonCAR_clicked();
};

#endif // MAINWINDOW_H
