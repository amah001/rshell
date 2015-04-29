CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic

all: rshell cp
	 
rshell:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/test.cpp -o bin/rshell
cp:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/cp.cpp -o bin/cp
clean:
	rm -rf ./bin
