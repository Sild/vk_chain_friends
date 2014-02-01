#ifndef QUEUE_H
#define QUEUE_H

#include <map>
#include <vector>
#include <QMutex>

class Link {
public:
    Link(int n, Link* p, int l) : number(n), parent(p), level(l) {}
    int lvl() const { return level; }
    Link* prt() const { return parent; }
    int n() const { return number; }
private:
    int number;
    Link* parent;
    int level;
};


class Queue
{
public:
    static Queue& ins() {
        static Queue q;
        return q;
    }

    Link* get(int number) const;
    Queue& push(int parent, std::vector<int> elems);
    int next() {
        if (iterator < s) return container.at(iterator++)->n();
        return 0;
    }

    bool setNext(std::vector<int>& block, unsigned size = 20);

    int isAim(int number) {
        return number == 659061;
    }

private:
    Queue() : iterator(0), s(0) {}
    Queue(const Queue&) {}
    void operator=(const Queue&) {}

private:
    std::map<int, Link*> index;
    std::vector< Link* > container;
    unsigned iterator;
    unsigned s;

    QMutex mutex;
};

#endif // QUEUE_H
