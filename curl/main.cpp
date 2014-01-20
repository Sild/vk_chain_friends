#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <sstream>
#include <boost/thread.hpp>
#include "json.h"
#include <algorithm>
#include <ctime>
// #include <mutex>

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

boost::mutex mtx_wagon;
boost::mutex mtx_content;


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

const int MAX_THREAD_COUNT = 1;
const int MAX_WAGON_PER_THREAD = 1000;
int current_thread_count = 0;
int root_id = 0;
int target_id = 0;
bool view_showed = false;
bool target_founded = false;






void threading();

struct Wagon {
	Wagon(int _id, Wagon* _parent, int _deep):id(_id), parent(_parent), deep(_deep){}
	int id;
	Wagon* parent;
	int deep;
};

class Train {
public:
	void show() {
		std::cout << "[" << content.size() << "]";
		for(int i = 0; i < content.size(); i++)	{
			std::cout << content[i] << "; ";
		}
		std::cout << "show finish" << std::endl;
	} 
	Train() {
		iterator = 0;
	}

	bool is_finish() {
		if(iterator < content.size()) {
			return false; 
		} else {
			return true;
		}
	}

	std::vector<int> get_next() {
		std::vector<int> ids;
		if (iterator < content.size()) {
			if((content.size() - iterator)  < MAX_WAGON_PER_THREAD) {
				for(int i = iterator; i < content.size(); i++) {
					ids.push_back(content[i]);
				}
				iterator = content.size();
			} else {
				for(int i = iterator; i < iterator + MAX_WAGON_PER_THREAD; i++) {
					ids.push_back(content[i]);
				}
				iterator += MAX_WAGON_PER_THREAD;
				
			}
			return ids;
		}
		return ids;
	}

	Wagon* get(int id) {
		std::map<int, Wagon* >::iterator itr = wagons.find(id);
		if(itr != wagons.end()) {
			return itr->second;
		} else {
			return 0;
		}
	}

	void hitch(Wagon* parent, std::vector<int> ids) {
		int par_id = (parent)?parent->id:root_id;
		std::cout << "hitch " << ids.size() << " elements from " << par_id << std::endl << "new content size: " << content.size() << std::endl;
		// mtx_wagon.lock();
		// mtx_content.lock();
		for(int i = 0; i < ids.size(); i++) {
			if(wagons.find(ids[i]) == wagons.end()) {
				Wagon* wagon = new Wagon(ids[i], parent, (parent)?parent->deep + 1:0);
				wagons[ids[i]] = wagon;
				content.push_back(ids[i]);
			}
		}
		// mtx_wagon.unlock();
		// mtx_content.unlock();
	}

private:
	std::map<int, Wagon*> wagons;
	std::vector<int> content;
	int iterator;
};


Train* train = new Train();


void init() {
	std::cout << "you can set 0 for root&target to get test-chain(from 659061 to 681449)" << std::endl;
	std::cout << "root_id: ";
	std::cin >> root_id;
	std::cout << "target_id: ";
	std::cin >> target_id;
	if(root_id == 0 && target_id == 0) {
		root_id = 659061;
		target_id = /*1117;//*/681449;
	}
	std::vector<int> ids;
	ids.push_back(root_id);
	train->hitch(0, ids);
}

int conductor(std::vector<int> ids) {
	if(ids.size() > 0) {
		for(int i = 0; i < ids.size(); i++) {
			std::vector<int> fr = Finder::friends(ids[i]);
			if(std::find(fr.begin(), fr.end(), target_id)!=fr.end()) {
				target_founded = true;
				train->hitch(train->get(ids[i]), fr);
				current_thread_count--;
				return ids[i];
			} else {
				train->hitch(train->get(ids[i]), fr);
			}	
		}
		current_thread_count--;
		return 0;
	} else {
		current_thread_count--;
		return 0;
	}
	
}
boost::thread_group threads;

void show_result(int finder);

void threading() {
	if(!target_founded) {
		while((current_thread_count < MAX_THREAD_COUNT) & !train->is_finish()){ 

			threads.create_thread(boost::bind(conductor, train->get_next()));
			current_thread_count++;
		}
		threads.join_all();
		threading();
	} else {
		show_result(target_id);
	}
}

void show_result(int target_id) {
	Wagon* target_wagon = train->get(target_id);
	while(target_wagon->id != root_id) {
		std::cout << target_wagon->id << " << ";
		target_wagon = target_wagon->parent;
	}
	std::cout << target_wagon->id;
	std::cout << std::endl;
}




int main()
{
	clock_t start = clock();
	init();
   	threading();

    clock_t finish = clock();
    
    // std::cout << (double)(finish - start)<< std::endl;
    return 0;
}




