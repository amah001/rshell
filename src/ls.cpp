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
#include <iomanip>
#include <string>
#include <strings.h>
#include <cctype>

using namespace std;
/*
int findWidth(vector<string> files)
{
	int width = 0;
	string size = 0;
	for(unsigned int i = 0; i < files.size();i++)
	{
		struct stat info;
		if(stat(files[i].c_str(),&info) == -1)
		{
			perror("stat");
			_exit(1);
		}
		
		if(width < info.st_size)
		{
			cout << info.st_size;
			width = info.st_size;
		}
	}
	return width;
}
*/

bool comparision(char  a,char b)
{
	return tolower(a)<tolower(b);
}

bool compararer(string A, string B)
{
	return lexicographical_compare(A.c_str(),A.c_str() + A.size(),B.c_str(),B.c_str() + B.size(),comparision);
}

void directoryRunthrough(vector<string>& files, char* argv[], string function, const char* path)
{
	DIR *dirp;
	if((dirp = opendir(".")) == NULL)
	{
		perror("opendir()");
		_exit(1);
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
		_exit(1);
	}
	if(closedir(dirp) == -1)
	{
		perror("closedir()");
		_exit(1);
	}
	sort(files.begin(),files.end(),compararer);

	return;
}
void print(char*argv[],char** path,int numPath)
{
	vector<string> files;
	string directoryz = "ls";
	string dota = ".";
	directoryRunthrough(files,argv,directoryz,path[numPath]);
	for(unsigned int i = 0; i < files.size();i++)
	{
		if(files[i][0] != '.')
		{
			cout << files[i] << endl;
		}
	}
	return;
}
void printAll(char* argv[],char** path,int numPath)
{
	vector<string> files;
	string directoryz = "a";
	directoryRunthrough(files,argv,directoryz,path[numPath]);
	for(unsigned int i = 0; i < files.size();i++)
	{
		cout << files[i] << endl;
	}
	return;
}
void printLong(char* argv[],char** path,int numPath,bool printAll)
{
	vector<string> files;
	string directoryz = "l";
	vector<string> permissions;
	directoryRunthrough(files,argv,directoryz,path[numPath]);
	if(!printAll)
	{
		vector<string>::iterator my_it = files.begin();
		for(unsigned int i = 0; i < files.size();i++)
		{
				
			if(files[i][0] == '.')
			{
				files.erase(my_it);
				i--;
				my_it--;
			}
			my_it++;
		}
	}
	//cout << "doge" << endl;
	for(unsigned int i = 0; i < files.size(); i++)
	{
		
		struct stat info;
		struct passwd *pwd;
		struct group *grp;
		string shortTime;
		if(stat(files[i].c_str(),&info) == -1)
		{
			perror("stat");
			_exit(1);
		}
		if((pwd = getpwuid(info.st_uid)) == NULL)
		{
			perror("getpwuid()");
			_exit(1);
		}
		if((grp = getgrgid(info.st_gid)) == NULL)
		{
			perror("getgrgid()");
			_exit(1);
		}
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
		shortTime = ctime(&info.st_mtime);
		shortTime = shortTime.substr(4,12);
		cout << info.st_nlink << " ";	//prints # of links
		cout << pwd->pw_name << " ";	//prints user
		cout << grp->gr_name << " ";	//prints group
		cout << setw(7) << right << info.st_size << " "; //prints size
		cout << shortTime << " ";	//prints time last modified
		cout << files[i] << endl;	//prints file
	}
	return;
}
void printRecursive(char* argv[],char** path,int numPath, bool runAll, bool runLong)
{
	vector<string> files;
	string directoryz = "R";
	vector<string> my_directorys;
	directoryRunthrough(files,argv,directoryz,path[numPath]);
	unsigned int index = 0;
	if(runAll)
	{
		index = 2;	//so it doesn't do . and ..
	}
	if(files.size() > 0)
	{
		cout << path[numPath] << ":" << endl;

	}
	for(unsigned int i = index; i < files.size(); i++)
	{
		string the_path_taken;
		the_path_taken = path[numPath] + '/' + files[i];
		directoryRunthrough(files,argv,directoryz,path[numPath]);
	}
	for(unsigned int i = 0; i < my_directorys.size(); i++)
	{
		//printRecursive(argv,my_directorys[i].c_str(),runAll,runLong);
	}
	
	return;
}

int main(int argc, char* argv[])
{
	vector<string> lists;
	bool runAll = false;
	bool runLong = false;
	bool runRecursive = false;
	bool supported = true; 	//checks to see if there is a bad flag
	int lastFlagLocation = 0;
	for(int i = 1; i < argc; i++)
	{
		string argument;
		argument = argv[i];
		if(argument.find("-") == 0)
		{
			for(unsigned int j = 1; j < argument.size(); j++)
			{
				if(argument.at(j) == 'a')
				{
					runAll = true;
				}
				else if(argument.at(j) == 'l')
				{
					runLong = true;
				}
				else if(argument.at(j) == 'R')
				{
					runRecursive = true;
				}
				else
				{
					supported = false;
				}

			}
			lastFlagLocation = i;
		}
	}
	if(!supported)
		return 0;
	//path finder
	char** path = (char**)malloc(BUFSIZ);
	size_t i = lastFlagLocation + 1;
	size_t j = 0;
	size_t currentPath = 0;
	for(; i < (unsigned)argc; i++, j++)
	{
		//each j is a different path
		path[j] = argv[i];
	}
	path[j] = NULL;		//so that the syscall thing works
	if (argc == 1)
	{
		print(argv,path,0);
	}
	else
	{
		//none of them work
		do
		{
			if(!runAll && !runLong && !runRecursive)
			{
				//no flags
				cout << "non" << endl;
				print(argv,path,currentPath);
			}
			else if(runAll && !runLong && !runRecursive)
			{
				// only -a
				printAll(argv,path,currentPath);
			}
			else if(!runAll && runLong && !runRecursive)
			{
				cout << "lead" << endl;
				// only -l
				printLong(argv,path,currentPath, runAll);
			}
			else if(!runAll && !runLong && runRecursive)
			{
				//only -R
				//printAll(argv);
			}
			else if(runAll && runLong && !runRecursive)
			{
				// -la or -al
				printLong(argv,path,currentPath,runAll);
			}
			else if(runAll && !runLong && runRecursive)
			{
				//-ar or -ra
				//printAll(argv);
			}
			else if(runAll && !runLong && !runRecursive)
			{
				// -lr or rl
				//printAll(argv);
			}
			else if(runAll && runLong && runRecursive)
			{
				// -lra or -rla or etc etc 
				//printAll(argv);
			}
			currentPath++;
			//cout << currentPath << endl;
			//cout << j << endl;
			
		}while(currentPath < j);
	}
	return 0;
}
