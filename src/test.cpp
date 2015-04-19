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
void parser(string &string_input)
{
    int commentLocater = string_input.find("#");
		if(commentLocater != -1)
		{
			string_input = string_input.substr(0,commentLocater);
		}
        //cout << string_input << endl;
        string::iterator my_iterator;
        int position = 0;
        
        for( my_iterator = string_input.begin();
        my_iterator < string_input.end();
        my_iterator++, position++)
        {
            // puts spaces around connectors
            int NextFind = string_input.find(";",position);
                //looks for ;
            int ifWorkFind = string_input.find("&&",position);
                //looks for &&
            int ifFailFind = string_input.find("||",position);
                //looks for ||
            if(((NextFind < ifWorkFind && NextFind < ifFailFind) 
                || (ifWorkFind == -1 && ifFailFind ==-1)
                || (NextFind < ifWorkFind && ifFailFind ==-1)
                || (NextFind < ifFailFind && ifWorkFind ==-1))
                && (NextFind != -1))
            {
                
                //adds spaces around ; connector
                string_input.insert(NextFind, " ");
                string_input.insert(string_input.find(";",position)+1," ");
                my_iterator = string_input.begin() + string_input.find(";")+1;
                position = string_input.find(";", position)+1;
            }
            else if(((ifWorkFind < NextFind && ifWorkFind < ifFailFind) 
                || (NextFind == -1 && ifFailFind ==-1)
                || (ifWorkFind < NextFind && ifFailFind ==-1)
                || (ifWorkFind < ifFailFind && NextFind ==-1))
                && (ifWorkFind != -1))
            {
                
                //adds spaces around && connector
                string_input.insert(ifWorkFind, " ");
                string_input.insert(string_input.find("&&",position)+2," ");
                my_iterator = string_input.begin() + string_input.find("&&")+2;
                position = string_input.find("&&", position)+2;
            }
            else if(((ifFailFind < ifWorkFind && ifFailFind < NextFind) 
                || (ifWorkFind == -1 && NextFind ==-1)
                || (ifFailFind < ifWorkFind && NextFind ==-1)
                || (ifFailFind < NextFind && ifWorkFind ==-1))
                && (ifFailFind != -1))
            {
                
                //adds spaces around || connector
                string_input.insert(ifFailFind, " ");
                string_input.insert(string_input.find("||",position)+2," ");
                my_iterator = string_input.begin() + string_input.find("||")+2;
                position = string_input.find("||", position)+2;
            }
            //cout << string_input << endl;
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
        parser(command_input);  //parses stuff

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
