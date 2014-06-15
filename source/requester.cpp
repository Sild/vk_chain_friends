#include "../header/requester.h"
#include <sstream>

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ostringstream *stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    return count;
}


void Requester::operator()(const User* parent, DIRECTION dir, std::vector<User> &cont)
{
    //std::cout << "Start friends get" << std::endl;
    std::stringstream req;
    req << "https://api.vk.com/method/friends.get?user_id=" << parent->id;
    std::ostringstream buffer;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, req.str().c_str() );;
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            //std::cout << "Nothing to return" << std::endl;
          return;
        }
    }
    std::string result( buffer.str() );


    if(result.length()) {
        json::Object obj(json::Deserialize(result));
        json::Value response(obj["response"]);
        if (response.GetType() == json::ArrayVal) {
            json::Array ids(response);
            for (
                 std::vector< json::Value >::const_iterator iter = ids.begin(), end = ids.end();
                 iter != end; ++iter
                 ) {
                if (iter->IsNumeric()) {
                    cont.push_back(User(*iter, dir, parent));
                }
            }
        }
    }
    //std::cout << "Return friends" << std::endl;
}

std::string Requester::name(size_t id)
{
    //std::cout << "Start friends get" << std::endl;
    std::stringstream req;
    req << "https://api.vk.com/method/users.get?user_id=" << id;
    std::ostringstream buffer;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, req.str().c_str() );;
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
          return "";
    }
    std::string result( buffer.str() );


    if(result.length()) {
        json::Object obj(json::Deserialize(result));
        json::Value response(obj["response"]);
        if (response.GetType() == json::ArrayVal)
        {
            json::Array res(response);
            const json::Value hash(res[0]);
            if (hash.GetType() == json::ObjectVal)
            {
                std::string first(hash.operator []("first_name"));
                std::string last(hash.operator []("last_name"));
                return first + " " + last;
            }
        }
    }
    return "";
}
