#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include <QString>

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

    QTableWidgetItem *ent_;
    QString textEnt_;
    int line_ = 0;

};

#endif // MAINWINDOW_H
