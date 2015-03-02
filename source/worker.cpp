#include "../header/worker.h"
#include <sstream>
#include "../header/requester.h"

size_t Worker::num = 1;

Worker::Worker(const Range &range)
    : ThreadBase("thread"), r(range)
{
}

void Worker::run()
{
    std::vector<User> container;
    Requester req;
    for (size_t i = 0; i < r.len; ++i, ++r.begin)
        req(&(r.begin[i]), r.begin[i].dir, container);
    Container::getInstance().insert(container);
}
