CXX = g++
CXXFLAGS = -Wall -Werror -ansi -pedantic

all: rshell cp ls mv rm
	 
rshell:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/test.cpp -o ./bin/rshell
cp:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/cp.cpp -o ./bin/cp
ls:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/ls.cpp -o ./bin/ls
mv:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/mv.cpp -o ./bin/mv
rm:
	mkdir -p ./bin
	$(CXX) $(CXXFLAGS) ./src/rm.cpp -o ./bin/rm
clean:
	rm -rf bin
