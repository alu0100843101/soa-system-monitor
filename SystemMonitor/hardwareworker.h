#ifndef HARDWAREWORKER_H
#define HARDWAREWORKER_H

#include <QObject>

//Blog - Example:
//https://jmtorres.webs.ull.es/me/2013/02/hilos-de-trabajo-usando-senales-y-slots-en-qt/

class HardwareWorker : public QObject
{
    Q_OBJECT

//public slots:
    void hardProcess();
};

#endif // HARDWAREWORKER_H
