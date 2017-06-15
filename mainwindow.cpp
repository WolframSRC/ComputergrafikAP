#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->sldRotationZ,SIGNAL(valueChanged(int)),ui->widget,SLOT(receiveRotationZ(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
