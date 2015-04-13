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
void commandList(string input, vector<vector<strings> commands)
{
	
}
*/
int main(int argc, char**argv)
{
	//connectors
	string executeNext = ";";	//execute next command no matter what
	string ifWorkExecute = "&&";	//execute next command if previous command succeeds
	string ifFailExecute = "||";	//execute next command if previous command fails 
displayPrompt();
cout << " hi kenny" << endl;
/*
	while(1)
	{
		displayPrompt();
		string command_input;
		vector<vector<strings> commands_calls;
		getline(cin, command_input);

		break;
	}
	
*/













return 0;
}
