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
#include <errno.h>
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
bool exitCheck(char** exitCommand)
{
	if(*exitCommand == "exit")
	{
		return true;
	}
	else
	{
		return false;
	}
}
void separator_parser(string &string_input)
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
bool run_command(char** command_list)
{
        
        pid_t pid = fork();
        if(pid ==-1)
        {
            perror("fork");
            exit(1);
        }
        else if(pid == 0)
        {
            cout << "Children" << endl;
            if(execvp(command_list[0],command_list) == -1)
            {
                perror("execvp");
            }
            _exit(1);
        }
        else if(pid > 0)
        {
            if(wait(0) == -1)
            {
                perror("wait()");
            }
        }
        
	

}
 int main(int argc, char**argv) { 
	//connectors 
    /*
    string spaces = " ";
	string RunNext = ";";	    //run next command no matter what
	string ifWorkRun = "&&";	//run next command if previous command works
	string ifFailRun = "||";	//run next command if previous command fails
*/

	while(1)
	{
		displayPrompt();
		string command_input;
		//vector<char*> commands_calls;
		getline(cin,command_input);	//takes in input
        separator_parser(command_input);  //puts spaces between everything

		char* command_char = new char[command_input.size()];
		strcpy(command_char,command_input.c_str());
		//char **iterator_token = (char**)malloc(BUFSIZ);
        char** final_command = (char**)malloc(BUFSIZ);
        char* iterator_token = strtok(command_char," ");
        //iterator_token[0] = temp_token;
        //iterator_token[1] = NULL;
        final_command[0] = iterator_token;
        final_command[1] = NULL;


        char** temp_char = (char**)malloc(BUFSIZ);
        string currentConnector;
        bool workRun = true;
        bool FailRun = false;
        bool containsArgument = false;
        bool containsConnectors = false;
             
        int counter = 0;
        /*
        while(iterator_token != NULL)
        {
            char** temporary_token;

            while(iterator_token != NULL && containsConnectors == false)
            {     
                string tempString = iterator_token;
                cout << tempString << endl;
                if((tempString == ";")
                    ||(tempString == "&&") 
                    || (tempString == ";"))
                {
                    //checks if there is a connector
                    containsConnectors = true;
                }
                if(containsConnectors = true)
                {
                    temp_char[counter] = NULL;
                    //NULL after each command in order to work in execvp
                    currentConnector = tempString;
                }
                else
                {
                    temp_char[counter] = iterator_token;
                }
                
                //if(exitCheck(iterator_token));
                //{
                //    exit(1);
                //}
                //
                counter++;
                iterator_token = strtok(NULL, " ");
                if(currentConnector == ";")
                {

                }
                else if(currentConnector == "&&")
                {

                }
                else if(currentConnector == "||")
                {

                }
                else
                {
                    //run_command()
                }
                //iterator_token = strtok(NULL," ");
              
            }
        final_command = temp_char;
        */
        //char
        run_command(final_command);
        /*
        pid_t pid = fork();
        if(pid ==-1)
        {
            perror("fork");
            exit(1);
        }
        else if(pid == 0)
        {
            cout << "Children" << endl;
            if(execvp(iterator_token[0],iterator_token) == -1)
            {
                perror("execvp");
            }
            _exit(1);
        }
        else if(pid > 0)
        {
            if(wait(0) == -1)
            {
                perror("wait()");
            }
        }
        */
	}
	return 0;
}
