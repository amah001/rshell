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
//#include <boost/tokenizer.hpp>
using namespace std;
//using namespace boost;
void displayPrompt()
{
	if(getlogin() == NULL)
	{
		perror("getlogin");
		_exit(1);
	}

	char host[BUFSIZ];
	if(gethostname(host,BUFSIZ) == -1)
	{
		perror("getlogin");
		_exit(1);
	}

	cout << getlogin() << "@" << host  << ":~$";
	return;
}
bool exitCheck(char* exitCommand)
{
	if(exitCommand == "exit")
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*
void commandList(*char input, vector<vector<string> > commands)
{
	while(input != NULL)
	{
		commands.push_back(input);
	}
	return;
} */ int main(int argc, char**argv) { 
	//connectors 
	string executeNext = ";";	//execute next command no matter what
	string ifWorkExecute = "&&";	//execute next command if previous command succeeds
	string ifFailExecute = "||";	//execute next command if previous command fails


	while(1)
	{
		displayPrompt();
		string  command_input;
		vector<char*> commands_calls;
		getline(cin,command_input);	//takes in input
		char* first_token;
		char* command_char = (char*)command_input.c_str();
		//strcpy(command_charz,command_input.c_str());;
		cout << command_char << endl;
		command_char = strtok(command_char," ");	
		//char* test = (char*)command_char;
		char* temp_token;
		size_t s_find = command_input.find(executeNext);
		size_t a_find = command_input.find(ifWorkExecute);
		size_t o_find = command_input.find(ifFailExecute);
		while(command_char != NULL)
		{
			cout << command_char << endl;
			command_char = strtok(NULL," ");
		}

		int pid = fork();
		if (pid == -1)
		{
			perror("fork");
			_exit(1);
		}
		else if(pid == 0)
		{
			cout << "This is the child process" << endl;
			
			if(-1 == execvp(argv[0], argv))		//if execvp fails
			{
			       perror("execvp");
			}
			
			exit(1);
		}
		else if(pid > 0)
		{
			if(wait(0) == -1)
			{
				perror("wait(0)");
			}
		}

		//break;
}

	

return 0;
}
