class Config
{
public:
	// static Config get_instance();

	Config(int root, int target);
	Config(int root, int target, int threads_count);
	int get_threads_count();
	int get_max_wagons();
	void inc_threads_count();
	void dec_threads_count();
	int get_root();
	int get_target();
private:
	// static Config get_instance = 0;
	int root_id;
	int target_id;
	int threads_count;
	int MAX_THREAD_COUNT;
	int MAX_WAGON_PER_THREAD;
};