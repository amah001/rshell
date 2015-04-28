#include "Timer.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

void  methodOne(char *argv[3])
{
	Timer t;
	double eTime;
	t.start();
	ifstream is(argv[1]);
	ofstream outfile(argv[2]);

	if(is.is_open())
	{
		char c = is.get();
		while(is.good())
		{
			outfile.put(c);
			c = is.get();
		}
	}
	is.close();
	outfile.close();
	t.elapsedUserTime(eTime);
	cout << eTime << endl;
	return;
}


void methodTwo(char *argv[3])
{
	Timer t;
	double eTime;
	t.start();
	int fdnew;
	int fdold;
	if((fdold = open(argv[1],O_RDONLY)) == -1)
	{
		perror("open()");
		exit(1);
	}
	if((fdnew = open(argv[2],O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR)) == -1)
	{
		perror("open()");
		exit(1);
	}
	int size;
	char c[1];

	if((size = read(fdold,&c,sizeof(c))) == -1)
	{
		perror("read()");
		exit(1);
	}
	
	while(size > 0)
	{
		
		if((write(fdnew,c,size)) ==-1)
		{
			perror("write()");
			exit(1);
		}
		if((size = read(fdold,&c,sizeof(c))) == -1)
		{
			perror("read()");
			exit(1);
		}
	}
	t.elapsedUserTime(eTime);
	cout << eTime << endl;
	
	return;
}


void methodThree(char* argv[3])
{
	Timer t;
	double eTime;
	t.start();
	int fdnew;
	int fdold;
	if((fdold = open(argv[1],O_RDONLY)) == -1)
	{
		perror("open()");
		exit(1);
	}
	if((fdnew = open(argv[2],O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR)) == -1)
	{
		perror("open()");
		exit(1);
	}
	int size;
	char c[BUFSIZ];

	if((size = read(fdold,c,sizeof(c))) == -1)
	{
		perror("read()");
		exit(1);
	}
	
	while(size > 0)
	{
		
		if((write(fdnew,c,size)) ==-1)
		{
			perror("write()");
			exit(1);
		}
		if((size = read(fdold,&c,sizeof(c))) == -1)
		{
			perror("read()");
			exit(1);
		}
	}
	t.elapsedUserTime(eTime);
	cout << eTime << endl;
	
	return;
}

int main(int argc, char *argv[]){

	ifstream i(argv[1]);
	if(i.good())
	{
		i.close();
	}
	else
	{
		i.close();
		cout << "file doesn't exist" << endl;
		exit(1);
	}
	ifstream f(argv[2]);
	if(f.good())
	{
		f.close();
		cout << "file exists" << endl;
		exit(1);
	}
	else
	{
		f.close();
	}
	if(argc == 4)
	{
		methodOne(argv);
		methodTwo(argv);
		methodThree(argv);
	}
	else
	{
		cout << "fastest" << endl;
		methodThree(argv);
	}
	
	return 0;
}
