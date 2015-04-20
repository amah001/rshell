CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic

all: rshell
	 
rshell:
	mkdir bin
	$(CXX) $(CXXFLAGS) ./src/test.cpp -o ./bin/rshell

clean:
	rm -rf bin
