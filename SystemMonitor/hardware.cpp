#include "hardware.h"

//Blog - Example:
//https://jmtorres.webs.ull.es/me/2013/02/hilos-de-trabajo-usando-senales-y-slots-en-qt/

Hardware::Hardware(QObject *parent) : QObject(parent)
{
    //Nothing
}

void Hardware::hardProcess()
{
    QProcess proc_;
    proc_.start("lshw -json");
    proc_.waitForFinished();
    proc_.waitForReadyRead();
    byteArray_ = proc_.readAllStandardOutput();

    emit hardwareRequest();
}

QByteArray Hardware::hardwareAsync()
{
    return byteArray_;
}
