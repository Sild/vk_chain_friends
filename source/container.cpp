#include "../header/container.h"
#include "../header/requester.h"
#include <unistd.h>
#include <cstdlib>
#include <cerrno>

Range Container::getRange()
{
    //std::cout << "start range formed" << std::endl;
    size_t start = iterator;
    int mutex_result;
    while((mutex_result = pthread_mutex_lock(&size_mutex)) != 0)
    {
        if (mutex_result == EBUSY) {
            std::cout << "mutex busy, wait..." << std::endl;
            sleep(1);
        }
        else {
            std::cout << "error " << mutex_result << std::endl;
            exit(1);
        }
    }
    size_t length = ((size - iterator >= MAX_RANGE)
                     ? (MAX_RANGE)
                     : (size - iterator));
    pthread_mutex_unlock(&size_mutex);
    iterator += length;
    //std::cout << "range return" << std::endl;
    return Range(cont + start, length);
}

void Container::insert(const std::vector<User> &range)
{
    //std::cout << "start insert" << std::endl;
    int mutex_result;
    while((mutex_result = pthread_mutex_lock(&size_mutex)) != 0)
    {
        if (mutex_result == EBUSY) {
            std::cout << "mutex busy, wait..." << std::endl;
            sleep(1);
        }
        else {
            std::cout << "error " << mutex_result << std::endl;
            exit(1);
        }
    }
    size_t index = size;
    size += range.size();
    if (size > MAX_SIZE) {
        std::cerr << "Too much iterations" << std::endl;
        exit(1);
    }
    pthread_mutex_unlock(&size_mutex);

    for (size_t i = 0; i < range.size(); ++i)
    {
        cont[index + i] = range[i];
        if (find(index, index + i))
            return;
    }

    //std::cout << "insert new portion\n";
}


void Container::show()
{
    if (finded)
    {
        const User *first = cont + chain.first;
        std::list<const User*> list;
        while(first)
        {
            list.push_back(first);
            first = first->parent;
        }
        const User *second = cont[chain.second].parent;
        while(second)
        {
            list.push_front(second);
            second = second->parent;
        }
        Requester req;
        for(std::list<const User*>::iterator i = list.begin();
                i != list.end(); ++i)
        {
            std::string name(req.name((*i)->id));
            if (name.size() != 0)
                std::cout << name << " ";
            std::cout << "(id" << (*i)->id << ")" << std::endl;
        }

    }
}
