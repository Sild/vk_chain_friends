#include "worker.h"
#include "container.h"
#include "requester.h"
#include <unistd.h>
#include "pthread.h"
#include <sstream>
#include <cstdlib>

struct thread_info
{
    pthread_t id;
    bool stop;
    thread_info() : id(0), stop(true) {}
};

#define MAX_THREADS 5
thread_info threads[MAX_THREADS];

void *thread_func(void *a);

void *run_threads(void *a)
{
    if (!Container::getInstance().isFinded())
    {
        for (size_t i = 0; i < MAX_THREADS; ++i)
        {
            if (threads[i].stop)
            {
                threads[i].stop = false;
                pthread_create(&threads[i].id, NULL, thread_func, (void *)i);
                pthread_detach(threads[i].id);
            }
        }
    }
    return NULL;
}


void *thread_func(void *a)
{
    int num = (int)a;
    //std::cout << "Thread start" << std::endl;
    Range r = Container::getInstance().getRange();
    std::vector<User> cont;
    Requester req;
    for (size_t i = 0; i < r.len; ++i)
        req(&(r.begin[i]), r.begin[i].dir, cont);
    if (cont.size() > 0)
        Container::getInstance().insert(cont);
    //std::cout << "Thread end" << std::endl;
    threads[num].stop = true;
    pthread_t loop;
    pthread_create(&loop, NULL, run_threads, NULL);
    pthread_detach(loop);
    return NULL;
}

void kill_threads()
{
    int killed = 0;
    for (size_t i = 0; i < MAX_THREADS; ++i)
        if (!threads[i].stop)
        {
            pthread_cancel(threads[i].id);
            ++killed;
        }
    std::cout << "killed " << killed << " threads" << std::endl;
}


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " <from> <to>" << std::endl;
        return 0;
    }
    size_t from = atoi(argv[1]);
    size_t to = atoi(argv[2]);
    if (from == 0 || to == 0)
    {
        std::cout << "can't recognize id" << std::endl;
        return 0;
    }
    Container::getInstance().setFinders(from, to);

    run_threads(NULL);

    while(!Container::getInstance().isFinded())
        usleep(10);
    kill_threads();

    Container::getInstance().show();


    return 0;
}



