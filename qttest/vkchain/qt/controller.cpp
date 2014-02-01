#include "controller.h"

Controller::Controller(QObject *parent) :
    QObject(parent), max(30)
{
    std::vector<int> q;
    q.push_back(681449);
    Queue::ins().push(0, q);
}
