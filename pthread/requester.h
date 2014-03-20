#ifndef REQUESTER_H
#define REQUESTER_H

#include <curl/curl.h>
#include "json.h"
#include "container.h"
#include <iostream>
#include <string>

class Requester
{
private:
    Requester(const Requester& ){}
    void operator=(const Requester &) {}
public:
    Requester() : curl(curl_easy_init()) {}
    void operator()(const User* parent, DIRECTION dir, std::vector<User> &cont);
    std::string name(size_t id);

private:
    CURL* curl;
    CURLcode res;

};

#endif // REQUESTER_H
