#include "threadbase.h"

ThreadBase::ThreadBase(const char *_threadName/* = NULL*/)
        : is_running(false), stop_requested(false), threadName("")
{
    if (_threadName != NULL)
        threadName = _threadName;
    memset(&thread, 0, sizeof(thread));
}

/* Method to start thread execution */
int ThreadBase::start()
{
    int retval = -1;
    if (!is_running)
        if (0 == pthread_create(&thread, NULL, &thread_func, (void *)this))
        {
            is_running = true;
            retval = 0;
        }
    return retval;
}


/* Method to stop thread execution */
int ThreadBase::stop()
{
    if (is_running)
        stop_requested = true;
    return 0;
}


int ThreadBase::join()
{
    int retval = -1;
    if (is_running)
    {
        if (0 == pthread_join(thread, NULL))
            retval = 0;
    }
    else
        retval = 0;
    return retval;
}


int ThreadBase::cancel()
{
    int retval = -1;
    if (is_running)
    {
        if (0 == pthread_cancel(thread))
        {
            is_running = false;
            retval = 0;
        }
    }
    else
        retval = 0;
    return retval;
}


/* Thread main function (it is just calling run() method)*/
void *ThreadBase::thread_func(void *arg)
{
    if (arg != NULL)
    {
        ThreadBase *p_thread_object = (ThreadBase*)arg;
        p_thread_object->stop_requested = false;
        p_thread_object->run();
        /* Reset running state if returned from run() method */
        p_thread_object->is_running = false;
        p_thread_object->stop_requested = false;
    }
    return NULL;
}
