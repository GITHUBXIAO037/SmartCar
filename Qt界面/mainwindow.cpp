#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myTimer = new QTimer(this);
    pictureSocket = new QTcpSocket(this);
    controlSocket = new QTcpSocket(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startTime(){
    myTimer->start(200);
}

void MainWindow::sendcmd(){
    pictureSocket->write("PIC");
    pictureSocket->flush();
}

void MainWindow::getpic(){

    int ret;
    char response[20];
    char *len;
    unsigned int piclen;
    char picbuf[1024 * 1024];

    memset(response, 0, sizeof(response));
    ret = pictureSocket->read(response, sizeof(response));
    if (ret != sizeof(response)) {
        qDebug() << "recv response failed";
        pictureSocket->close();

    }

    len = strstr(response, "len");
    if (len == NULL) {
        qDebug() << "response header is error";
        pictureSocket->close();
        return;
    }

    *len = '\0';
    piclen = atoi(response);

    qDebug() << "piclen: " << piclen;

    int total = 0;
    while (total < piclen) {
    //    qDebug()<<"while 68";
        ret = pictureSocket->read(picbuf + total, piclen - total);
        if (ret < 0) {
            qDebug() << "recv pic failed" << ret;
           pictureSocket->close();
            return;
        } else if (ret == 0) {
            pictureSocket->waitForReadyRead(30000);

        } else
            total += ret;
  //  qDebug()<<"while 78";
    }

    qDebug() << "total: " << total;

    QPixmap pixmap;

    pixmap.loadFromData((const uchar *)picbuf, piclen, "JPEG");

    ui->label->setPixmap(pixmap);
}

void MainWindow::on_pushButtonConnect_clicked()
{

    pictureSocket->connectToHost("192.168.1.1",5858);
//     pictureSocket->connectToHost("192.168.5.111",5858);
    qDebug()<<"connected OK";
    connect(pictureSocket,SIGNAL(connected()),this,SLOT(startTime()));

    connect(pictureSocket,SIGNAL(readyRead()),this,SLOT(getpic()));

    connect(myTimer,SIGNAL(timeout()),this,SLOT(sendcmd()));
}

void MainWindow::tips(){

    qDebug()<<"car connected OK";

}
void MainWindow::on_pushButtonCAR_clicked()
{
    controlSocket->connectToHost("192.168.1.1",5959);

    connect(controlSocket,SIGNAL(connected()),this,SLOT(tips()));
}

void MainWindow::STOP(){
    char buf[2] ="0";
    controlSocket->write(buf,sizeof(buf));
}

void MainWindow::UP(){
    char buf[3] = "VU";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::DOWN(){
    char buf[3] = "VD";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::LEFT(){
    char buf[3] = "HL";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::RIGHT(){
    char buf[3] = "HR";
    controlSocket->write(buf,sizeof(buf));
}
/*-------------car control-----------------*/

void MainWindow::goHead(){
    char buf[2] ="1";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goBack(){
    char buf[2] ="2";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goLeft(){
    char buf[2] ="3";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goRight(){
    char buf[2] ="4";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goLeftHead(){
    char buf[2] ="5";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goLeftBack(){
    char buf[2] ="6";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goRightHead(){
    char buf[2] ="7";
    controlSocket->write(buf,sizeof(buf));
}
void MainWindow::goRightBack(){
    char buf[2] ="8";
    controlSocket->write(buf,sizeof(buf));
}

void MainWindow::on_pushButton_LEFT_pressed()
{
    LEFT();
}

void MainWindow::on_pushButton_UP_pressed()
{
    UP();
}

void MainWindow::on_pushButton_DOWN_pressed()
{
    DOWN();
}

void MainWindow::on_pushButton_RIGHT_pressed()
{
    RIGHT();
}

void MainWindow::on_pushButton_RIGHT_released()
{

}

/*-----------------------------小车方向-----------------------------------------------*/

void MainWindow::on_pushButtonCARFRONT_pressed()
{
    goHead();
}

void MainWindow::on_pushButtonCARFRONT_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARLF_pressed()
{
    goLeftHead();
}

void MainWindow::on_pushButtonCARLF_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARRF_pressed()
{
    goRightHead();
}

void MainWindow::on_pushButtonCARRF_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARLEFT_pressed()
{
    goLeft();
}

void MainWindow::on_pushButtonCARLEFT_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARRIGHT_pressed()
{
    goRight();
}

void MainWindow::on_pushButtonCARRIGHT_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARLB_pressed()
{
    goLeftBack();
}

void MainWindow::on_pushButtonCARLB_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARBACK_pressed()
{
    goBack();
}

void MainWindow::on_pushButtonCARBACK_released()
{
    STOP();
}

void MainWindow::on_pushButtonCARRB_pressed()
{
    goRightBack();
}

void MainWindow::on_pushButtonCARRB_released()
{
    STOP();
}

