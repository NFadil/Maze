SRC_DIR = src/

all: maze


maze: main.o
	g++ main.o -o maze

main.o:
	g++ -c ${SRC_DIR}main.cpp

clean:
	rm -f main.o
