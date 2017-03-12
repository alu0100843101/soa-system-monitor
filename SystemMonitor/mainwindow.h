#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <QString>
#include <sys/types.h>      //Información de una cuenta de usuario a través de su nombre o id.
#include <pwd.h>            //Información de una cuenta de usuario a través de su nombre o id.
#include <QVector>          //Almacenar proceso en una estructura cola o vector.

#include "mythread.h"

namespace Ui {
class MainWindow;
}

class MyThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void dataReceive();

private:
    Ui::MainWindow *ui;

    MyThread *mythread_;

    QTableWidgetItem *entradaSensor_;
    QString textSensor_;
    int line_ = 0;

};

#endif // MAINWINDOW_H
