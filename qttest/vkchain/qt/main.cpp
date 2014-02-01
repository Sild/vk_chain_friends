#include <QCoreApplication>
#include "controller.h"
#include "worker.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Controller* c = new Controller();
    c->run();


    return a.exec();
}
