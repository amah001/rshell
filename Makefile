CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic

all: rshell cp ls
	 
rshell:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/test.cpp -o ./bin/rshell
cp:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/cp.cpp -o ./bin/cp
ls:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/ls.cpp -o ./bin/ls

clean:
	rm -rf bin
