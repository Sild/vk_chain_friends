#include "../Headers"

Train::Train() {
	iterator = 0;
}

Wagon* Train::get_next() {
	if (iterator < contaiter.size()) {
		Wagon* tmp = contaiter[iterator];
		iterator++;
		return tmp;
	}
	return 0;
}

Wagon* Train::get(int id) {
	//Wagon* wagon = 0;
	std::map<int, Wagon* >::iterator itr = consist.find(id);
	if(itr != consist.end()) {
		return itr->second;
	} else {
		return 0;
	}
}

void Train::hitch(Wagon* parent, std::vector<int> ids) {
	for(int i = 0; i < ids.size(); i++) {
		if(consist.find(ids[i]) == consist.end()) {
			Wagon* wagon = new Wagon(ids[i], parent, (parent)?parent->deep + 1:0);
			consist[ids[i]] = wagon;
			std::cout << "hitch: " << ids[i] << std::endl;
			contaiter.push_back(wagon);
		}
	}
}