all: main.o json.o
	clang++ main.o json.o -o app -lcurl -lboost_system -lboost_thread -g


main.o: main.cpp
	clang++ -c main.cpp -o main.o -g -pg

json.o: json.cpp
	clang++ -c json.cpp -o json.o -g -pg

clean:
	rm -r *.o app

