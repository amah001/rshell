#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <vector>
#include <cstring>
#include <string.h>
#include <map>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <algorithm>

using namespace std;

void directoryRunthrough(vector<string> files, char* argv[], string function)
{
	DIR *dirp;
	if(function == "ls")
	{	
		if((dirp = opendir(".")) == NULL)
		{
			perror("opendir()");
			exit(1);
		}
		
	}
	else
	{
		if((dirp = opendir(argv[1])) == NULL)
		{
			perror("opendir()");
			exit(1);
		}
	}
	struct dirent *filespecs;
	errno = 0;
	while ((filespecs = readdir(dirp)) != NULL)
	{
		files.push_back(filespecs->d_name);
	}
	if(errno != 0)
	{
		perror("readdir()");
		exit(1);
	}
	if(closedir(dirp) == -1)
	{
		perror("closedir()");
		exit(1);
	}
	sort(files.begin(),files.end());

	return;
}
void print(char*argv[])
{
	vector<string> files;
	string directoryz = "ls";
	directoryRunthrough(files,argv,directoryz);
	for(int i = 0; i < files.size();i++)
	{
	//	if(files[i][0] == '.')
	//	{
			cout << files[i] << endl;
	//	}
	}
	return;
}
void printAll(char* argv[])
{
	return;
}
void printLong()
{

}
void printRecursive()
{

}

int main(int argc, char* argv[])
{
	vector<string> lists;
	if(argc == 2)
	{
		print(argv);
	}

	/*
	for(int i = 0; i < lists.size(); i++)
	{
		cout << lists[i] << endl;
	}
	*/
	return 0;
}
