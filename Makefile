.PHONY: main

all: main

main:
	g++ main.cpp -std=c++0x -lzmq -o main

run:
	./main

clean:
	rm -rf main
