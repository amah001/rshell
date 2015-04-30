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
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
using namespace std;

void directoryRunthrough(vector<string>& files, char* argv[], string function)
{
	DIR *dirp;
	//if(function == "ls")
	//{
	//	cout << "run"<< endl;
		if((dirp = opendir(".")) == NULL)
		{
			perror("opendir()");
			_exit(1);
		}
		
	//}
	/*
	else
	{
		if((dirp = opendir(argv[1])) == NULL)
		{
			perror("opendir()");
			_exit(1);
		}
	}
	*/
	struct dirent *filespecs;
	errno = 0;
	while ((filespecs = readdir(dirp)) != NULL)
	{
		files.push_back(filespecs->d_name);
	}
	if(errno != 0)
	{
		perror("readdir()");
		_exit(1);
	}
	if(closedir(dirp) == -1)
	{
		perror("closedir()");
		_exit(1);
	}
	sort(files.begin(),files.end());

	return;
}
void print(char*argv[])
{
	vector<string> files;
	string directoryz = "ls";
	directoryRunthrough(files,argv,directoryz);
	for(unsigned int i = 0; i < files.size();i++)
	{
		if(files[i][0] != '.')
		{
			cout << files[i] << endl;
		}
	}
	return;
}
void printAll(char* argv[])
{
	vector<string> files;
	string directoryz = "a";
	directoryRunthrough(files,argv,directoryz);
	return;
}
void printLong(char* argv[])
{
	vector<string> files;
	string directoryz = "l";
	cout << "h" << endl;
	directoryRunthrough(files,argv,directoryz);
	struct stat info;
	struct passwd *pwd;
	struct group *grp;
	string owner;
	string group;
	int links;
	cout << "1h" << endl;
	string dot = ".";
	if(stat(dot.c_str(),&info) == -1)
	{
		perror("stat");
		_exit(1);
	}
	cout << "2h" << endl;
	if((pwd = getpwuid(info.st_uid)) == NULL)
	{
		perror("getpwuid()");
		_exit(1);
	}
	cout << "3h" << endl;
	owner = pwd->pw_name;
	if((grp = getgrgid(info.st_gid)) == NULL)
	{
		perror("getgrgid()");
		_exit(1);
	}
	cout << "4h" << endl;
	group = grp->gr_name;
	if(S_ISREG(info.st_mode))
		cout << "-";
	else if(S_ISDIR(info.st_mode))
		cout << "d";
	else if(S_ISCHR(info.st_mode))
		cout << "c";
	else if(S_ISBLK(info.st_mode))
		cout << "b";
	else if(S_ISFIFO(info.st_mode))
		cout << "p";
	else if(S_ISLNK(info.st_mode))
		cout << "l";
	else if(S_ISSOCK(info.st_mode))
		cout << "s";

	if(S_IRUSR & info.st_mode)
		cout << "r";
	else
		cout << "-";
	if(S_IWUSR & info.st_mode)
		cout << "w";
	else
		cout << "-";
	if(S_IXUSR & info.st_mode)
		cout << "x";
	else
		cout << "-";

	if(S_IRGRP & info.st_mode)
		cout << "r";
	else
		cout << "-";
	if(S_IWGRP & info.st_mode)
		cout << "w";
	else
		cout << "-";
	if(S_IXGRP & info.st_mode)
		cout << "x";
	else
		cout << "-";

	if(S_IROTH & info.st_mode)
		cout << "r";
	else
		cout << "-";
	if(S_IWOTH & info.st_mode)
		cout << "w";
	else
		cout << "-";
	if(S_IXOTH & info.st_mode)
		cout << "x ";
	else
		cout << "- ";
	//*************************************************
	// do some alignment thingy
	cout << info.st_nlink << " ";	//prints # of links
	cout << pwd->pw_name << " ";	//prints user
	cout << grp->gr_name << " ";	//prints group
	cout << info.st_blksize << " "; //prints size
	cout << ctime(&info.st_mtime) << " ";
	cout << files[0] << endl;
	return;
}
void printRecursive()
{

}

int main(int argc, char* argv[])
{
	vector<string> lists;
	if(argc == 1)
	{
		print(argv);
	}
	printLong(argv);
	/*
	for(int i = 0; i < lists.size(); i++)
	{
		cout << lists[i] << endl;
	}
	*/
	return 0;
}
