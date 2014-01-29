#include "wagon.h"
#include "config.h"
#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <vector>
#include <sstream>
#include <boost/thread.hpp>
class Train {
public:
	void show();

	Train();

	bool is_finish();

	std::vector<int> get_next();

	Wagon* get(int id);

	void hitch(Wagon* parent, std::vector<int> ids);

private:
	std::map<int, Wagon*> wagons;
	std::vector<int> content;
	int iterator;
	boost::mutex mutex;
};