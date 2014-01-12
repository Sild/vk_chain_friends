#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <sstream>
<<<<<<< HEAD
#include <boost/thread.hpp>
=======
#include "json.h"

>>>>>>> a10623b2e8cecacf212b5977c4f2a7261a10edda
class Request {
public:
    Request() : curl(curl_easy_init()) {}
    std::string run( const std::string& url );

private:
    CURL* curl;
    CURLcode res;

};

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::ostringstream *stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    return count;
}

std::string Request::run( const std::string &url ) {
    std::ostringstream buffer;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );//"https://api.vk.com/method/friends.get?user_id=2");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
          return "";
        curl_easy_cleanup(curl);
    }

    return buffer.str();
}

class Finder {
public:
    static std::vector<int> friends(int user_id);
private:
    static Request req;
};

std::vector<int> Finder::friends(int user_id) {
    std::ostringstream buf;
    buf << "https://api.vk.com/method/friends.get?user_id=" << user_id;
    std::string result( req.run(buf.str()) );
    std::cout << result << std::endl;
    std::vector<int> r;
    return r;
}

Request Finder::req;

void long_function() {
	printf("Thread: Hello!\n");
    boost::posix_time::seconds SleepTime(2);
    boost::this_thread::sleep(SleepTime);	//	Ждем 2 секунды (подробее см. ниже)
    printf("Thread: Bye Bye!");
}

int main()
{
    Finder::friends(659061);
    boost::thread Thread(long_function);
    Thread.join();
    return 0;
}



