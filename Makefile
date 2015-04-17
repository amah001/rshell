CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic

all: rshell
	 
rshell:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/test.cpp -o src/bin/rshell

clean:
	rm -rf ./bin
