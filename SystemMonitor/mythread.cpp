#include "mythread.h"

MyThread::MyThread(int tiempo, QObject *parent)
    : QThread(parent)
{
    time_ = tiempo;
}

int MyThread::mySize()
{
    return fileInfoList_.size();
}

void MyThread::push(QPair <QString, QString> pair)
{
//Qt Documentation: http://doc.qt.io/qt-4.8/qmutexlocker.html
    QMutexLocker locker(&mutex_);
    queue_.enqueue(pair);
    emit dataSend();
}

QPair<QString,QString> MyThread::pop()
{
   QMutexLocker locker(&mutex_);
   return queue_.dequeue();
}

void MyThread::run()
{
//Information about QDir and QFileInforList: https://supportforums.blackberry.com/t5/Native-Development/How-to-read-all-files-in-a-folder-with-QDir-entryList/td-p/2678661
    QPair<QString, QString> pair;
    QFile file_;
    QByteArray contain_;
    QDir dir_;

    dir_.setPath("/sys/class/hwmon");           //Directorio donde están los ficheros con la info de los sensores.
    dir_.setNameFilters(QStringList()<<"hwmon*"
                        <<"temp*"<<"name");     //Los archivos que buscamos son temp* y fan*
                                                //Este puede estar en carpetas hwmon*

    QStringList fileList_ = dir_.entryList();
    QFileInfoList myFileInfoList_;

    while(true){
        for(auto entry: fileList_) {
           dir_.cd(entry);
           myFileInfoList_ = dir_.entryInfoList(QDir::AllEntries | QDir::Hidden);
           fileInfoList_=myFileInfoList_;
           for(auto archivos: myFileInfoList_){

               file_.setFileName(archivos.absoluteFilePath());
               file_.open(QIODevice::ReadOnly | QIODevice::Text);

               contain_=file_.readAll();
               pair.first=archivos.fileName();
               pair.second=contain_.trimmed();
               push(pair);

               file_.close();

//Qt Documentation: https://doc.qt.io/archives/4.6/qthread.html
               msleep(time_);                   //Puede usarse msleep() o sleep()
               //sleep(time_);                  //sleep() cuenta el tiempo en segundos y msleep en milisegundos.
            }

           dir_.cd("..");
        }
    }
}
