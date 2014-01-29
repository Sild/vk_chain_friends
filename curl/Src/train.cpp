#include "../Hdrs/train.h"
extern config;

void  Train::show() {
	std::cout << "[" << content.size() << "]";
	for(int i = 0; i < content.size(); i++)	{
		std::cout << content[i] << "; ";
	}
	std::cout << "show finish" << std::endl;
}

Train::Train() {
	iterator = 0;
}

bool Train::is_finish() {
	mutex.lock();
	if(iterator < content.size()) {
		mutex.unlock();
		return false; 
	} else {
		mutex.unlock();
		return true;
	}
}

std::vector<int> Train::get_next() {
	std::vector<int> ids;
	if (iterator < content.size()) {
		mutex.lock();
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
		mutex.unlock();
		return ids;
	}
	return ids;
}

Wagon* Train::get(int id) {
	mutex.lock();
	std::map<int, Wagon* >::iterator itr = wagons.find(id);
	if(itr != wagons.end()) {
		mutex.unlock();
		return itr->second;
	} else {
		mutex.unlock();
		return 0;
	}
}

void Train::hitch(Wagon* parent, std::vector<int> ids) {
	for(int i = 0; i < ids.size(); i++) {
		if(wagons.find(ids[i]) == wagons.end()) {
			Wagon* wagon = new Wagon(ids[i], parent, (parent)?parent->deep + 1:0);
			mutex.lock();
			wagons[ids[i]] = wagon;
			content.push_back(ids[i]);
			mutex.unlock();
		}
	}
	int par_id = (parent)?parent->id:root_id;
	std::cout << "hitch " << ids.size() << " elements from " << par_id << std::endl << "new content size: " << content.size() << std::endl;
}
