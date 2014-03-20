all: obj main.o
	clang++ -o find requester.o json.o container.o main.o -lpthread -lcurl

obj: json.cpp json.h requester.cpp requester.h container.cpp container.h
	clang++ -c json.cpp requester.cpp container.cpp

main.o: main.cpp container.h requester.h
	clang++ -c main.cpp -o main.o

clean:
	rm -f *.o
