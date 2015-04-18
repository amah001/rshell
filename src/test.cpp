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
		
        int commentLocater = command_input.find("#");
		if(commentLocater != -1)
		{
			command_input = command_input.substr(0,commentLocater);
		}
        cout << command_input << endl;
        string::iterator my_iterator;
        int position;
        /*
        for( my_iterator = command_input.begin(),my_iterator < command_input.end(),my_iterator++)
        {
            int NextFind = command_input.find(executeNext);     //looks for ;
            int ifWorkFind = command_input.find(ifWorkExecute);   //looks for &&
            int ifFailFind = command_input.find(ifFailExecute);   //looks for ||
            if(NextFind < ifWorkFind && NextFind < ifFailFind & NextFind != -1)
            {
                //adds spaces around ; connector
                command_input.insert(NextFind, " ");
                command_input.insert(command_input.find(executeNext)+1," ")
                my_iterator = command_input.find(executeNext)+1;
            }
        }
        */
		char* command_char = new char[command_input.size()];
		strcpy(command_char,command_input.c_str());
		char *iterator_token = strtok(command_char," ");
		while(iterator_token !=0)
		{
			cout << iterator_token << endl;
			iterator_token = strtok(NULL," ");
			/*
            if(exitCheck(iterator_token));
			{
				exit(1);
			}
            */
		}
	}
	return 0;
}
