#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <sstream>
#include <boost/thread.hpp>
#include "json.h"
#include <algorithm>

class Request {
public:
    Request() : curl(curl_easy_init()) {}
    std::string run( const std::string& url );
    ~Request() {
    	curl_easy_cleanup(curl);
    }
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


    std::vector<int> r;
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
	                r.push_back(*iter);
	            }
	        }
	    }
    }

    //std::cout << result << std::endl;
    return r;
}

Request Finder::req;



void threading();

struct Wagon {
	Wagon(int _id, Wagon* _parent, int _deep):id(_id), parent(_parent), deep(_deep){}
	int id;
	Wagon* parent;
	int deep;
};

class Train {
public:
	Train() {
		iterator = 0;
	}

	Wagon* get_next() {
		/*if(is_init) {
			is_init = false;
			itrtr = consist.begin();
		} else {
			itrtr++;			
		}
		if(itrtr == consist.end()) {
			return 0;
		} else {
			Wagon* tmp = (itrtr->second);
			return tmp;
		}*/

		if (iterator < contaiter.size()) {
			Wagon* tmp = contaiter[iterator];
			iterator++;
			return tmp;
		}
		return 0;
	}

	Wagon* get(int id) {
		//Wagon* wagon = 0;
		std::map<int, Wagon* >::iterator itr = consist.find(id);
		if(itr != consist.end()) {
			return itr->second;
		} else {
			return 0;
		}
	}

	void hitch(Wagon* parent, std::vector<int> ids) {
		for(int i = 0; i < ids.size(); i++) {
			if(consist.find(ids[i]) == consist.end()) {
				Wagon* wagon = new Wagon(ids[i], parent, (parent)?parent->deep + 1:0);
				consist[ids[i]] = wagon;
				contaiter.push_back(wagon);
			}
		}
	}

private:
	bool is_init;
	std::map<int, Wagon*> consist;
	std::map<int, Wagon*>::iterator itrtr;

	std::vector< Wagon* > contaiter;
	int iterator;
};

const int MAX_THREAD_COUNT = 100;
const int MAX_WAGON_PER_THREAD = 100;
int current_thread_count = 0;
int root_id = 0;
int target_id = 0;
bool view_showed = false;
bool target_founded = false;

Train* train = new Train();

void init() {
	root_id = 117611;
	target_id = 1;//681449;
	std::vector<int> ids;
	ids.push_back(root_id);
	train->hitch(0, ids);


}

int conductor(std::vector<int> ids) {
		//	std::cout << "cinductor" << std::endl;

	for(std::vector<int>::iterator itr = ids.begin(), end = ids.end(); itr != end; ++itr) {
		// std::cout << "chenit'" << *itr << std::endl;
		std::vector<int> fr = Finder::friends(*itr);
		std::cout << fr.size() << std::endl;
		/*for(std::vector<int>::iterator asd = fr.begin(); asd != fr.end(); asd++) {
			std::cout << *asd << ", ";
		}*/
		std::cout << std::endl;
		//if(std::find(fr.begin(), fr.end(), target_id)!=fr.end()) {
			//target_founded = true;
			// std::cout << "asdasdawweeeee";
			//current_thread_count--;
			//return *itr;
		//} else {
			train->hitch(train->get(*itr), fr);

			threading();
		//}	
	}
	current_thread_count--;

	return 0;
}

void threading() {
			std::cout << "testthreadbegin" << std::endl;

	if(!target_founded) {

		while(current_thread_count < MAX_THREAD_COUNT) {
			if(!target_founded) {
				std::vector<int> queue;
				for(int i = 0; i < MAX_WAGON_PER_THREAD; i++) {
					Wagon* wagon = train->get_next();
					if(wagon) {
						
						if (wagon->id == target_id) {
							while (1) std::cout << "IIHHHAA" << std::endl;
						}
						queue.push_back(wagon->id);

					} else {
						break;
					}
				}
				boost::thread Thread(conductor, queue);
	    		Thread.join();
	    		current_thread_count++;
			} else {
				break;
			}
			
		}
	} else {
		if(!view_showed) {
			std::cout << "zaebca" << std::endl;
			view_showed = true;
		}
	}
}







int main()
{
	init();
    threading();
    return 0;
}



