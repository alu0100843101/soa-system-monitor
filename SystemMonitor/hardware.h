#ifndef HARDWARE_H
#define HARDWARE_H

//Blog - Example:
//https://jmtorres.webs.ull.es/me/2013/02/hilos-de-trabajo-usando-senales-y-slots-en-qt/

#include<QByteArray>
#include<QProcess>
#include<QDebug>

#include "hardwareworker.h"

class Hardware : public QObject
{
    Q_OBJECT

public:
    Hardware(QObject *parent = nullptr);
    //~Hardware();

    QByteArray hardwareAsync();

signals:
    void hardwareRequest();

public slots:
    void hardProcess();

private:
    QByteArray byteArray_;
    //HardwareWorker hardWorker_;
};

#endif // HARDWARE_H
