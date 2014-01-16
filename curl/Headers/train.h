class Train {
public:
	Train();

	Wagon* get_next();

	Wagon* get(int id);

	void hitch(Wagon* parent, std::vector<int> ids);

private:
	bool is_init;
	std::map<int, Wagon*> consist;
	std::map<int, Wagon*>::iterator itrtr;

	std::vector< Wagon* > contaiter;
	int iterator;
};