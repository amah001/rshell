#include<unistd.h>
#include<fcntl.h>
#include<iostream>
#include<vector>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
using namespace std;

int main(int argc, char* argv[])
{
	struct stat s;
	struct stat d;
	if(stat(argv[1],&s) == -1)
	{
		perror("stat");
	}
	if(stat(argv[2],&d) == -1)
	{
		//perror("file does not exit");
		link(argv[1],argv[2]);
		unlink(argv[1]);
	}
	//if(!(S_ISDIR(s.st_mode)) && !(S_ISREG(s.st_mode)))
	//{
		//cout << "INHERE";
	//	link(argv[1],argv[2]);
	//}
	if(S_ISDIR(d.st_mode))
	{	
		//cout << "GOT HERE";
		string temp(argv[2]);
		temp += '/';
		temp += argv[1];
		link(argv[1],temp.c_str());
		unlink(argv[1]);
	}
	else
	{
		cout << "Files Exists" << endl;
	}
	return 0;
}
