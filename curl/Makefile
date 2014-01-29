all: main.o json.o
	clang++ main.o json.o -o app -lcurl -lboost_system -lboost_thread -g


main.o: main.cpp
	clang++ -c main.cpp -o Obj/main.o -g -pg

json.o: json.cpp
	clang++ -c json.cpp -o Obj/json.o -g -pg

train.o: Obj/config.o Obj/wagon.o
	clang++ -c Src/train.cpp -o Obj/train.o

config.o: Src/config.cpp
	clang++ -c Src/config.cpp -o Obj/config.o

wagon.o: Src/wagon.cpp
	clang++ -c Src/wagon.cpp -o Obj/wagon.o

clean:
	rm -r Obj/*.o app

