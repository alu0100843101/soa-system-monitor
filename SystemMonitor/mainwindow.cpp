#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int tiempo = 10;

    mythread_ = new MyThread(tiempo,this);
    mythread_->start();

    connect(mythread_, SIGNAL(dataSend()),this, SLOT(dataReceive()));

    ui->tableWidget_4->setColumnCount(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataReceive()
{
    QPair<QString,QString> pair;
    pair = mythread_->pop();

    if(textSensor_!=""){
        if(pair.second!=textSensor_)
        {
            if(ui->tableWidget_4->item(line_,0)==NULL)
            {
                ui->tableWidget_4->insertRow(line_);

                ui->tableWidget_4->setItem(line_,0,new QTableWidgetItem(pair.first));
                ui->tableWidget_4->setItem(line_,1,new QTableWidgetItem(pair.second));

                line_++;

            } else
            {
                ui->tableWidget_4->setItem(line_,0,new QTableWidgetItem(pair.first));
                ui->tableWidget_4->setItem(line_,1,new QTableWidgetItem(pair.second));
            }

            line_++;
        } else
        {
            line_ = 0;

            ui->tableWidget_4->setItem(line_,0,new QTableWidgetItem(pair.first));
            ui->tableWidget_4->setItem(line_,1,new QTableWidgetItem(pair.second));

            line_++;
        }

    } else
    {
        ui->tableWidget_4->insertRow(line_);

        ui->tableWidget_4->setItem(line_,0,new QTableWidgetItem(pair.first));
        ui->tableWidget_4->setItem(line_,1,new QTableWidgetItem(pair.second));

        entradaSensor_ = ui->tableWidget_4->item(0,1);
        textSensor_ = entradaSensor_->text();

        line_++;
    }
}
