#ifndef THREADBASE_H
#define THREADBASE_H

#include <string>
#include <pthread.h>
#include <cstring>

class ThreadBase
{
public:
    ThreadBase(const char *_threadName = NULL);
    virtual ~ThreadBase() { stop(); }

    int start();
    int stop();
    int join();
    int cancel();

    bool isRunning() { return is_running; }

protected:
    /* Abstract run() method shall be implemented by ancestor class */
    virtual void run() = 0;

    bool isStopRequested() const { return stop_requested; }

private:
    bool is_running;
    bool stop_requested;
    pthread_t thread;
    std::string threadName;
    static void *thread_func(void *arg);
};

#endif // THREADBASE_H
