#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <iostream>
#include "worker.h"
#include "queue.h"
#include <QThread>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    void run() {
        std::vector<int> r;
        for (int i = 0; i < max; ++i) {
            r.clear();
            if (Queue::ins().setNext(r)) {
                --max;
                Worker* w = new Worker(r);
                QThread* thread = new QThread();
                w->moveToThread(thread);

                connect( thread, SIGNAL(started()), w, SLOT(process()));
                connect( w, SIGNAL(finished()), thread, SLOT(quit()));
                connect( w, SIGNAL(finished()), w, SLOT(deleteLater()));
                connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

                connect( w, SIGNAL(finished()), this, SLOT(moreThread()));

                connect( w, SIGNAL(finded()), this, SLOT(URA()));
                thread->start();
            }
            else break;
        }
    }

signals:

public slots:
    void moreThread() {
        //std::cout << "IHHA" << std::endl;
        ++max;
        run();
    }

    void URA() {
        std::cout << "fadsjls" << std::endl;
        exit(0);
    }

private:
    int max;

};

#endif // CONTROLLER_H
