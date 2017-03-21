#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <QString>
#include <sys/types.h>      //Información de una cuenta de usuario a través de su nombre o id.
#include <pwd.h>            //Información de una cuenta de usuario a través de su nombre o id.
#include <QVector>          //Almacenar proceso en una estructura cola o vector.
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
                            //Cada tarea se asigna al agrupamiento usando QtConcurrent.
#include <QFuture>          //QtConcurrent devuelve tipo QFuture.
#include <QTimer>           //Emite señales de temporizador cada intervalo de tiempo.

#include<QJsonDocument>
#include<QTreeWidget>
#include<QJsonObject>
#include<QJsonValue>

#include "mythread.h"
#include "hardware.h"

struct Process
{
    QString pid;
    QString cmdline;
    QString name;
    QString threads;
    //QString owner;
    int owner;
};

namespace Ui {
class MainWindow;
}

class MyThread;
//Qt Documentation: http://doc.qt.io/qt-5/qtimer.html#details
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Process get_proc(QDir p);

private slots:
    void dataReceive();
    void myTimeout();               //Señal timeout() [clase QTimer].
    //void handleFinished();
    void hardwareProcess();

private:
    Ui::MainWindow *ui;

    /* Parte 1: Sensores */
    MyThread *mythread_;

    QTableWidgetItem *entradaSensor_;
    QString textSensor_;
    int lineSensor_ = 0;

    /* Parte 2: Procesos */
    QVector <Process> procesos_;

    QTableWidgetItem *entradaProcesos_;
    QString textProcesos_;
    int lineProcesos_ = 0;

    QTimer *time_;

    passwd *psswd;

    /* Parte 3: Hardware */
    QThread hardThread_;
    Hardware hardProc_;
    QJsonDocument jdoc_;

    void addTreeRoot(QString name, QJsonDocument jdoc);
    void addTreeChild(QTreeWidgetItem *parent, QString name, QJsonObject obj);
};

#endif // MAINWINDOW_H
