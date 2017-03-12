#include "mainwindow.h"
#include "ui_mainwindow.h"

/********************** PUBLIC: **********************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int tiempo = 10;

    mythread_ = new MyThread(tiempo,this);
    mythread_->start();

    connect(mythread_, SIGNAL(dataSend()),this, SLOT(dataReceive()));

    ui->tableWidget_4->setColumnCount(2);       //Sensores.
    ui->tableWidget_4->setHorizontalHeaderLabels(QString("NOMBRE; INFO").split(";"));
    ui->tableWidget->setColumnCount(5);         //Procesos.
    ui->tableWidget->setHorizontalHeaderLabels(QString("NOMBRE; PID; CMDLINE; HILOS; PROPIETARIO").split(";"));

    //Qt Documentation: http://doc.qt.io/qt-5/qtimer.html#details
    time_ = new QTimer(this);
    connect(time_,SIGNAL(timeout()), this,
            SLOT(myTimeout()));                 //Se conectan las señales de timeout.
    time_->start(5000);                         //milisegundos -> 5000 mS = 5 segundos.
}

MainWindow::~MainWindow()
{
    delete ui;
}

Process MainWindow::get_proc(QDir p)
{
    Process proc_;
    QFile file_;
    QByteArray contain_;

    QFileInfoList fileInfoList_ = p.entryInfoList(QDir::AllEntries | QDir::Hidden);

    for(auto files_: fileInfoList_){
           file_.setFileName(files_.absoluteFilePath());
           file_.open(QIODevice::ReadOnly | QIODevice::Text);

           if(files_.fileName()=="cmdline"){
              contain_=file_.readLine();
              //Qt Documentation: http://doc.qt.io/qt-5/qstring.html#trimmed
              proc_.cmdline=contain_.trimmed();  //Espacios.
           }

           else if (files_.fileName()=="status")
           {
               while(file_.atEnd()){
                  while(file_.readLine(6)!="Name:"){
                   file_.readLine();
                  }
                  contain_=file_.readLine();
                  proc_.name=contain_.trimmed();

                  while(file_.readLine(5)!="Pid:"){
                   file_.readLine();
                  }
                  contain_=file_.readLine();
                  proc_.pid=contain_.trimmed();

                  while(file_.readLine(5)!="Uid:"){
                    file_.readLine();
                  }
                  contain_=file_.readLine(6);
                  //proc_.owner=contain_.trimmed();

                  proc_.owner=contain_.toInt();
                  file_.readLine();


                  while(file_.readLine(9)!="Threads:"){
                   file_.readLine();
                  }
                      contain_=file_.readLine();
                      proc_.threads=contain_.trimmed();

                }
           }

           file_.close();
     }
     return proc_;
}

/********************** PRIVATE SLOTS: **********************/

void MainWindow::dataReceive()
{
    QPair<QString,QString> pair;
    pair = mythread_->pop();

    if(textSensor_!=""){
        if(pair.second!=textSensor_)
        {
            if(ui->tableWidget_4->item(lineSensor_,0)==NULL)
            {
                ui->tableWidget_4->insertRow(lineSensor_);

                ui->tableWidget_4->setItem(lineSensor_,0,new QTableWidgetItem(pair.first));
                ui->tableWidget_4->setItem(lineSensor_,1,new QTableWidgetItem(pair.second));

                lineSensor_++;

            } else
            {
                ui->tableWidget_4->setItem(lineSensor_,0,new QTableWidgetItem(pair.first));
                ui->tableWidget_4->setItem(lineSensor_,1,new QTableWidgetItem(pair.second));
            }

            lineSensor_++;
        } else
        {
            lineSensor_ = 0;

            ui->tableWidget_4->setItem(lineSensor_,0,new QTableWidgetItem(pair.first));
            ui->tableWidget_4->setItem(lineSensor_,1,new QTableWidgetItem(pair.second));

            lineSensor_++;
        }

    } else
    {
        ui->tableWidget_4->insertRow(lineSensor_);

        ui->tableWidget_4->setItem(lineSensor_,0,new QTableWidgetItem(pair.first));
        ui->tableWidget_4->setItem(lineSensor_,1,new QTableWidgetItem(pair.second));

        entradaSensor_ = ui->tableWidget_4->item(0,1);
        textSensor_ = entradaSensor_->text();

        lineSensor_++;
    }
}

void MainWindow::myTimeout()
{
    QDir dir_;
    dir_.setPath("/proc");
    dir_.setNameFilters(QStringList()<<"1*"<<"2*"<<"3*"<<"4*"<<"5*"<<"6*"<<"7*"<<"8*"<<"9*"<<"status"<<"cmdline");
    QStringList fileList_ = dir_.entryList();

    for(auto entry: fileList_)
    {
        //Qt Documentation: doc.qt.io/qt-5/qfuturewatcher.html#QFutureWatcher
        QFutureWatcher <Process> *watcher_ = new QFutureWatcher<Process>;
        dir_.cd(entry);
        QFuture<Process> runProc_ = QtConcurrent::run(this,&MainWindow::get_proc,dir_);
        watcher_->setFuture(runProc_);

        //connect(&watcher_, SIGNAL(finished()), &QFutureWatcher<Process>, SLOT(handleFinished));
        connect(watcher_, &QFutureWatcher<Process>::finished,[=]()
        {
            //Linux Documentation: http://man7.org/linux/man-pages/man3/getpwnam.3.html
            //Qt Documetation: http://doc.qt.io/qt-5/qfuturewatcher.html#result
            psswd=getpwuid(runProc_.result().owner);

            if(psswd!=NULL)
            {
                qDebug() << "Ok";
            }
            else psswd=getpwuid(1000);

            if(textProcesos_!="")
            {
                 if(runProc_.result().pid!=textProcesos_)
                 {
                     if(ui->tableWidget->item(lineProcesos_,0)==NULL)
                     {
                         ui->tableWidget->insertRow(lineProcesos_);

                         ui->tableWidget->setItem(lineProcesos_,0,new QTableWidgetItem(runProc_.result().name));
                         ui->tableWidget->setItem(lineProcesos_,1,new QTableWidgetItem(runProc_.result().pid));
                         ui->tableWidget->setItem(lineProcesos_,2,new QTableWidgetItem(runProc_.result().cmdline));
                         ui->tableWidget->setItem(lineProcesos_,3,new QTableWidgetItem(runProc_.result().threads));
                         ui->tableWidget->setItem(lineProcesos_,4,new QTableWidgetItem(psswd->pw_name));

                         lineProcesos_++;
                     } else
                     {
                         ui->tableWidget->setItem(lineProcesos_,0,new QTableWidgetItem(runProc_.result().name));
                         ui->tableWidget->setItem(lineProcesos_,1,new QTableWidgetItem(runProc_.result().pid));
                         ui->tableWidget->setItem(lineProcesos_,2,new QTableWidgetItem(runProc_.result().cmdline));
                         ui->tableWidget->setItem(lineProcesos_,3,new QTableWidgetItem(runProc_.result().threads));
                         ui->tableWidget->setItem(lineProcesos_,4,new QTableWidgetItem(psswd->pw_name));

                         lineProcesos_++;
                     }
                 } else
                 {
                     lineProcesos_ = 0;

                     ui->tableWidget->setItem(lineProcesos_,0,new QTableWidgetItem(runProc_.result().name));
                     ui->tableWidget->setItem(lineProcesos_,1,new QTableWidgetItem(runProc_.result().pid));
                     ui->tableWidget->setItem(lineProcesos_,2,new QTableWidgetItem(runProc_.result().cmdline));
                     ui->tableWidget->setItem(lineProcesos_,3,new QTableWidgetItem(runProc_.result().threads));
                     ui->tableWidget->setItem(lineProcesos_,4,new QTableWidgetItem(psswd->pw_name));

                     lineProcesos_++;
                 }
            } else
            {
                 ui->tableWidget->insertRow(lineProcesos_);

                 ui->tableWidget->setItem(lineProcesos_,0,new QTableWidgetItem(runProc_.result().name));
                 ui->tableWidget->setItem(lineProcesos_,1,new QTableWidgetItem(runProc_.result().pid));
                 ui->tableWidget->setItem(lineProcesos_,2,new QTableWidgetItem(runProc_.result().cmdline));
                 ui->tableWidget->setItem(lineProcesos_,3,new QTableWidgetItem(runProc_.result().threads));
                 ui->tableWidget->setItem(lineProcesos_,4,new QTableWidgetItem(psswd->pw_name));

                 lineProcesos_++;

                 entradaProcesos_ = ui->tableWidget->item(0,1);
                 textProcesos_ = entradaProcesos_->text();
             }

             watcher_->waitForFinished();
             watcher_->deleteLater();
         });

        dir_.cd("..");
    }

}
