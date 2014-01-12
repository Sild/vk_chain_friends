all: main.o json.o
	clang++ main.o json.o -o app -lcurl

main.o: main.cpp
	clang++ -c main.cpp -o main.o

json.o: json.cpp
	clang++ -c json.cpp -o json.o

clean:
	rm -r *.o app