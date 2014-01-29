#include "../Hdrs/config.h"
// Config Config::get_instance()
// {
// 	if( !this->get_instance )
// 	{

// 	}
// }
Config::Config(int root, int target)
{
	this->root_id = root;
	this->target_id = target;
	this->MAX_THREAD_COUNT = 5;
	this->MAX_WAGON_PER_THREAD = 100;
	// this->instance = *this;

}

Config::Config(int root, int target, int threads_count) 
{
	this->root_id = root;
	this->target_id = target;
	this->MAX_THREAD_COUNT = threads_count;
	this->MAX_WAGON_PER_THREAD = 100;
	// this->instance = *this;
}

int Config::get_root()
{
	return this->root_id;
}

int Config::get_target()
{
	return this->target_id;
}

int COnfig::get_max_wagons()
{
	return this->MAX_WAGON_PER_THREAD;
}

int Config::get_threads_count()
{
	return this->threads_count;
}
	
void Config::inc_threads_count()
{
	++this->threads_count;

}

void Config::dec_threads_count()
{
	--this->threads_count;
}

