struct Wagon {
	Wagon(int _id, Wagon* _parent, int _deep);
	int id;
	Wagon* parent;
	int deep;
};