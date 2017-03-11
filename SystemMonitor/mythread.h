#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>      //Crear nuestro propio hilo para sensores, heredado de la clase QThread.
#include <QQueue>       //Almacenar los datos de cada sensor en una cola.
#include <QMutex>       //Mutex - Evitar condiciones de carrera.
#include <QDir>         //Recorrer directorios en busca de los datos de los sensores.
#include <QPair>        //Al igual que C, QT incluye una librería para crear estructuras de "pares de valores".
                        //Hay que almacenar el par etiqueta-valor.
#include <QString>      //Pares de tipo QString.
#include <QFileInfoList>
#include <QFile>
#include <QDebug>

class MyThread : public QThread
{
    Q_OBJECT

public:
    MyThread(int tiempo, QObject *parent = nullptr); //Varía en el tiempo.
    ~MyThread();

    int mySize();

    void push(QPair<QString,Qstring> pair);
    QPair<QString,Qstring> pop();

signals:
    void dataSend();

protected:
    void run() override; //Se sobreescribe el método run() de QThread.
private:
    QQueue< QPair<QString,QString> > queue_;
    QMutex mutex_;

    QFileInfoList fileInfoList_;

    int time_;
};

#endif // MYTHREAD_H
