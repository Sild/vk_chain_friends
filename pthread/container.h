#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <pthread.h>
#include <iostream>
#include <list>

#define MAX_SIZE 999999
enum DIRECTION { TO, FROM };

struct User
{
    int id;
    DIRECTION dir;
    const User * parent;
    User(int i, DIRECTION d, const User* p) : id(i), dir(d), parent(p) {}
    User() : id(0), dir(TO), parent(0) {}
};

struct Range
{
    const User *begin;
    size_t len;
    Range(User *start, size_t size) : begin(start), len(size) {}
};

class Container
{
    static const size_t MAX_RANGE = 100;
private:
    Container()
        : finded(false), iterator(0), size(0) {
        pthread_mutex_init(&size_mutex, NULL);
    }
    Container(const Container& c){}
    void operator=(const Container& c){}
public:
    ~Container() {
        pthread_mutex_destroy(&size_mutex);
    }
    static Container &getInstance()
    {
        static Container cont;
        return cont;
    }
    void setFinders(size_t _to, size_t _from) {
        if (_to != _from) {
            to = User(_to, TO, NULL);
            from = User(_from, FROM, NULL);
            cont[0] = to;
            cont[1] = from;
            size += 2;
        }
    }

    size_t full_size() const { return size; }

    Range getRange(); // use mutex
    void insert(const std::vector<User> &range); // use mutex

    // the same user, with equal id, but different direction
    // go to the parent of each user to get all chain
    bool find(size_t end, size_t pos)
    {
        if (finded) return true;
        DIRECTION dir = ((cont[pos].dir == TO)
                         ? FROM :
                           TO);
        for (size_t i = 0; i < end; ++i)
        {
            if (cont[i].id == cont[pos].id && cont[i].dir == dir)
            {
                chain = std::pair<size_t, size_t>(i, pos);
                return finded = true;
            }
        }
        return false;
    }

    bool isFinded() { return finded; }
    void show()
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
            for(std::list<const User*>::iterator i = list.begin();
                    i != list.end(); ++i)
            {
                std::cout << (*i)->id << std::endl;
            }
            
        }
    }


private:
    User cont[MAX_SIZE];
    //User *end;
    size_t iterator;
    size_t size;

    pthread_mutex_t size_mutex;
    std::pair<size_t, size_t> chain;
    bool finded;
    User to;
    User from;
};

#endif // CONTAINER_H
