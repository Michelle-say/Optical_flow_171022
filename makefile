CC = g++
OPENCV = `pkg-config opencv4 --cflags --libs`
EXE = opticalflow

all: main

main: main.o
	$(CC) -o ${EXE} main.o ${OPENCV}
 
main.o: main.cpp
	$(CC) -c main.cpp ${OPENCV}

clean:
	rm -Rf *.o
 