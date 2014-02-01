#include "queue.h"
#include <iostream>

Link* Queue::get(int number) const{
    std::map<int, Link*>::const_iterator elem = index.find(number);
    if (elem != index.end()) return elem->second;
    return 0;
}

Queue& Queue::push(int parent, std::vector<int> elems) {
    QMutexLocker lock(&mutex);
    unsigned size = elems.size();
    Link* elem = 0;
    Link* prn = 0;
    int cur = 0;
    for ( unsigned i = 0; i < size; ++i ) {
        cur = elems.at(i);
        if (get(cur) == 0) {
            prn = get(parent);
            elem = new Link(cur, prn, (prn ? prn->lvl() + 1 : 0 ));
            index[cur] = elem;
            container.push_back(elem);
        }
    }

    // need to block queue only here;
    s += size;

    return *this;
}

bool Queue::setNext(std::vector<int>& block, unsigned size) {
    QMutexLocker lock(&mutex);
#define OUT(D,V) std::cout << D << " " << V << std::endl
    unsigned count = (iterator + size < s) ? (size) : (s - iterator);
    iterator += count; // block only this line
    /*OUT("count", count);
    OUT("iterator", iterator);
    OUT("size", s);*/
    if ( count ) {
        for (unsigned i = iterator - count; i < iterator; ++i) {
            block.push_back(container.at(i)->n());
        }
        return true;
    }
    return false;
}
