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
				std::cout << "hitch: " << ids[i] << std::endl;
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
	std::cout << "root_id: ";
	std::cin >> root_id;
	std::cout << "target_id: ";
	std::cin >> target_id;
	if(root_id == 0 && target_id == 0) {
		root_id = 659061;
		target_id = 681449;
	}
	
	std::vector<int> ids;
	ids.push_back(root_id);
	train->hitch(0, ids);


}

int conductor(std::vector<int> ids) {
	for(std::vector<int>::iterator itr = ids.begin(), end = ids.end(); itr != end; ++itr) {
		std::vector<int> fr = Finder::friends(*itr);
		std::cout << fr.size() << std::endl;

		std::cout << std::endl;
		if(std::find(fr.begin(), fr.end(), target_id)!=fr.end()) {
			target_founded = true;
			current_thread_count--;
			return *itr;
		} else {
			train->hitch(train->get(*itr), fr);

			threading();
		}	
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
				std::vector< boost::thread* > threads;
				int counter = 0;
				for ( Wagon* wagon = train->get_next(); wagon; wagon = train->get_next(), counter++ ) {
					if ( counter < MAX_WAGON_PER_THREAD ) {
						queue.push_back(wagon->id);
					}
					else {
						threads.push_back(new boost::thread(conductor, queue));
						queue.clear();
						counter = 0;
					}
				}
				if (queue.size() > 0) {
					threads.push_back( new boost::thread(conductor, queue));
					queue.clear();
				}

				int thread_size = threads.size();
				for (int i = 0; i < thread_size; ++i, ++current_thread_count )
					threads[i]->detach();

				for (int i = 0; i < thread_size; ++i, ++current_thread_count )
					delete threads[i];
				/*for(int i = 0; i < MAX_WAGON_PER_THREAD; i++) {
					Wagon* wagon = train->get_next();
					if(wagon) {
						queue.push_back(wagon->id);

					} else {
						break;
					}
				}
				boost::thread Thread(conductor, queue);
	    		Thread.join();
	    		current_thread_count++;*/
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

void stop_all(int finder) {
	std::cout << "IHHAA: " << finder << std::endl;
	std::cout << target_id << " << ";
	Wagon* parent = 0;
	int prev = finder;
	for (Wagon* wagon = train->get(finder); wagon; wagon = wagon->parent) {
		std::cout << wagon->id << " << ";
	}
	std::cout << std::endl;
	while(true);
}

void validator(const std::vector<int>& ids) {
	size_t size = ids.size();
	for (size_t i = 0; i < size; ++i) {
		std::vector<int> fr = Finder::friends(ids[i]);
		if(std::find(fr.begin(), fr.end(), target_id)!=fr.end()) {
			stop_all(ids[i]);
		} else {
			train->hitch(train->get(ids[i]), fr);
		}	
	}
}

void non_theading() {
	Wagon* wagon = 0;
	int counter = 0;
	std::vector<int> queue;
	while( wagon = train->get_next()) {
		if (counter < MAX_WAGON_PER_THREAD) {
			queue.push_back( wagon->id );
		}
		else {
			validator(queue);
			queue.clear();
			counter = 0;
		}
		++counter;
	}
	if (queue.size() > 0) {
		validator(queue);
	}
}







int main()
{
	init();
	while(true) non_theading();
    //threading();
    return 0;
}



