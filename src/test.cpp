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
 int main(int argc, char**argv) { 
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
		char* command_char = new char[command_input.size()];
		strcpy(command_char,command_input.c_str());
		cout << command_char << endl;
		char *iterator_token = strtok(command_char," ");
		int commentLocater = command_input.find("#");
		if(commentLocater != -1)
		{
			command_input = command_input.substr(commentLocater);
		}
		while(iterator_token !=0)
		{
			cout << iterator_token << endl;
			iterator_token = strtok(NULL," ");
			if(exitCheck(iterator_token));
			{
				exit(1);
			}
		}
	}
	return 0;
}
