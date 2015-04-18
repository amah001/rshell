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
    string spaces = " ";
	string RunNext = ";";	    //run next command no matter what
	string ifWorkRun = "&&";	//run next command if previous command works
	string ifFailRun = "||";	//run next command if previous command fails


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
        int position = 0;
        
        for( my_iterator = command_input.begin();
        my_iterator < command_input.end();
        my_iterator++, position++)
        {
            // puts spaces around connectors
            int NextFind = command_input.find(RunNext,position);
                //looks for ;
            int ifWorkFind = command_input.find(ifWorkRun,position);
                //looks for &&
            int ifFailFind = command_input.find(ifFailRun,position);
                //looks for ||
            if(((NextFind < ifWorkFind && NextFind < ifFailFind) 
                || (ifWorkFind == -1 && ifFailFind ==-1)
                || (NextFind < ifWorkFind && ifFailFind ==-1)
                || (NextFind < ifFailFind && ifWorkFind ==-1))
                && (NextFind != -1))
            {
                
                //adds spaces around ; connector
                command_input.insert(NextFind, spaces);
                command_input.insert(command_input.find(RunNext,position)+1," ");
                my_iterator = command_input.begin() + command_input.find(RunNext)+1;
                position = command_input.find(RunNext, position)+1;
            }
            else if(((ifWorkFind < NextFind && ifWorkFind < ifFailFind) 
                || (NextFind == -1 && ifFailFind ==-1)
                || (ifWorkFind < NextFind && ifFailFind ==-1)
                || (ifWorkFind < ifFailFind && NextFind ==-1))
                && (ifWorkFind != -1))
            {
                
                //adds spaces around && connector
                command_input.insert(ifWorkFind, spaces);
                command_input.insert(command_input.find(ifWorkRun,position)+2," ");
                my_iterator = command_input.begin() + command_input.find(ifWorkRun)+2;
                position = command_input.find(ifWorkRun, position)+2;
            }
            else if(((ifFailFind < ifWorkFind && ifFailFind < NextFind) 
                || (ifWorkFind == -1 && NextFind ==-1)
                || (ifFailFind < ifWorkFind && NextFind ==-1)
                || (ifFailFind < NextFind && ifWorkFind ==-1))
                && (ifFailFind != -1))
            {
                
                //adds spaces around || connector
                command_input.insert(ifFailFind, spaces);
                command_input.insert(command_input.find(ifFailRun,position)+2," ");
                my_iterator = command_input.begin() + command_input.find(ifFailRun)+2;
                position = command_input.find(ifFailRun, position)+2;
            }
            //cout << command_input << endl;
        }
        
		char* command_char = new char[command_input.size()];
		strcpy(command_char,command_input.c_str());
		char *iterator_token = strtok(command_char," ");
		while(iterator_token !=0)
		{
			/*
            if(exitCheck(iterator_token));
			{
				exit(1);
			}
            */
			cout << iterator_token << endl;
            iterator_token = strtok(NULL," ");
            
		}
	}
	return 0;
}
