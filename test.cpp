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
		exit(1);
	}

	char host[BUFSIZ];
	gethostname(host,BUFSIZ);

	cout << getlogin() << "@" << host  << ":~$";
	return;
}
/*
void commandList(*char input, vector<vector<string> > commands)
{
	while(input != NULL)
	{
		commands.push_back(input);
	}
	return;
}
*/
int main(int argc, char**argv)
{
	//connectors
	string executeNext = ";";	//execute next command no matter what
	string ifWorkExecute = "&&";	//execute next command if previous command succeeds
	string ifFailExecute = "||";	//execute next command if previous command fails

//	char_separator<char> semicolon(executeNext);
//	char_separator<char> Ands(ifWorkExecute);
//	char_separator<char> Ors(ifFailExecute);
	while(1)
	{
		displayPrompt();
		string  command_input;
		vector<char*> commands_calls;
		getline(cin,command_input);	//takes in input
		char* first_token;
		char command_char[command_input.size()];
		strcpy(command_char,command_input.c_str());
//	commandList(command_input,command_calls);
		cout << command_input << endl;
		//char* test = (char*)command_char;
		char* temp_token;
		size_t s_find = command_input.find(executeNext);
		size_t a_find = command_input.find(ifWorkExecute);
		size_t o_find = command_input.find(ifFailExecute);
		if(s_find < a_find && s_find < o_find)
		{
			first_token = strtok(command_char,";");
			commands_calls.push_back(first_token);
		}
		else if(a_find < s_find && a_find < o_find)
		{
			first_token = strtok(command_char,"&&");	
			commands_calls.push_back(first_token);
		}
		else if(o_find < a_find && o_find < s_find)
		{
			first_token = strtok(command_char,"||");	
			commands_calls.push_back(first_token);
		}
		cout << commands_calls[0] << endl;
/*
for(int i = 0; i < command_input.size(); i++)
		{
			//size_t s_find = command_input.find(executeNext);
			//size_t a_find = command_input.find(ifWorkExecute);
			//size_t o_find = command_input.find(ifFailExecute);
		}
*/		break;
}

	

return 0;
}
