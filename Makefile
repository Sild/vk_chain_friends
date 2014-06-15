src=source/
hdr=header/
cmplr=clang++

all: obj main.o
	${cmplr} -o find requester.o json.o container.o main.o -lpthread -lcurl

obj: ${src}json.cpp ${hdr}json.h ${src}requester.cpp ${hdr}requester.h ${src}container.cpp ${hdr}container.h
	${cmplr} -c ${src}json.cpp ${src}requester.cpp ${src}container.cpp

main.o: ${src}main.cpp ${hdr}container.h ${hdr}requester.h
	${cmplr} -c ${src}main.cpp -o main.o

clean:
	rm -f *.o
