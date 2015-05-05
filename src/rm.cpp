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

void removeDir(const char* directory)
{
	vector<string>files;
	vector<string>dir;
	vector<string>input;
	DIR *dirp;
	if(NULL == (dirp = opendir(directory)))
	{
		perror("There was an error with opendir(). ");
		exit(1);
	}
	struct dirent *filespecs;
	errno = 0;
	while(NULL != (filespecs = readdir(dirp)))
	{
		input.push_back(filespecs->d_name);
	}
	if(errno != 0)
	{
		perror("There was an error with readdir(). ");
		exit(1);
	}
	if(-1 == closedir(dirp))
	{
		perror("There was an error with closedir(). ");
		exit(1);
	}
	for(int i = 2; i < input.size(); i++)
	{
		
		struct stat s;
		string tmp("./");
		//cout << directory << endl;
		tmp += directory;
		tmp+= input.at(i);
		//cerr << tmp << endl;
		if(-1==stat(tmp.c_str(),&s)) perror("open");
		else if(S_ISREG(s.st_mode))files.push_back(tmp);
		else if(S_ISDIR(s.st_mode)) dir.push_back(tmp);
	}
	//Remove files
	for(int i = 0; i < files.size(); i++)
	{
		if((unlink(files.at(i).c_str()))==-1) perror("unlink");
	}
	for(int i = 0; i < dir.size(); i++)
	{
		if(dir.at(i).at(dir.at(i).size()-1)!='/')
		{
			dir.at(i) = dir.at(i) + '/';
		}

		removeDir(dir.at(i).c_str());
		rmdir(dir.at(i).c_str());
	}
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Not enough argument" << endl;
		return -1;
	}
	else{
		vector<string> files;
		vector<string> directory;
		bool dashR = false;
		for(int i = 0; i < argc; i++)
		{
			//cout << " " << argv[i];
			struct stat s;
			if(-1==stat(argv[i],&s)) perror("open");
			if(strcmp( argv[i], "-r") == 0) dashR = true;//if(argv[i]=="-r") dashR = true;
			else if(S_ISREG(s.st_mode))files.push_back(argv[i]);
			else if(S_ISDIR(s.st_mode)) directory.push_back(argv[i]);
			//if(dashR) cout << ": dashR" << endl; 
		}
		//Remove files
		for(int i = 0; i < files.size(); i++)
		{
			if((unlink(files.at(i).c_str()))==-1) perror("unlink");
		}
		for(int i = 0; i < directory.size(); i++)
		{
			if(!dashR){
				if(-1==rmdir(directory.at(i).c_str())) cout << "Directory not empty" << endl;
			}
			if(dashR){
				//cout << "dashR" << endl;
				if(directory.at(i).at(directory.at(i).size()-1)!='/')
				{
					directory.at(i) = directory.at(i) + '/';
				}
				removeDir(directory.at(i).c_str());
				rmdir(directory.at(i).c_str());
			}
		}
	}


	return 0;
}
					   
