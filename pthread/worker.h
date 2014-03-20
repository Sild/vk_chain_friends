#ifndef WORKER_H
#define WORKER_H

#include "threadbase.h"
#include "container.h"

class Worker : public ThreadBase
{
public:
    Worker(const Range& range);
protected:

    virtual void run();

private:
    Range r;
    static size_t num;
};

#endif // WORKER_H
