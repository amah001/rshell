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

using namespace std;

void displayPrompt()
{   
    // shows username and hostname
    // BUFSIZ = BUFFERSIZE
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

bool exit_check(char** exitCommand)
{
    string tempz = exitCommand[0];
    size_t notExit = -1;
    if(tempz.find("exit") != notExit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void commentRemover(string &string_input)
{
    //removes comments from the input
    int commentLocater = string_input.find("#");
    if(commentLocater != -1)
    {
        string_input = string_input.substr(0,commentLocater);
    }
    return;
}

void separator_parser(string &string_input)
{
    commentRemover(string_input);
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
    }
    return;
}

string connector_parse(char**& command_lists, char*& current_token)
{
    // parses connectors such that they are separated by connectors only
    // this allows uses of arguments with commands 
    bool containsConnectors = false;
    size_t notFound = -1;
    string currentConnector;//connector used right before this command
    int counter = 0;
    while(current_token != NULL && containsConnectors == false)
    {
        string tempString = current_token;
        if(tempString.find(";") != notFound
            || tempString.find("&&") != notFound
            || tempString.find("||") != notFound)
        {
            //if there is a connector detected
            containsConnectors = true;
        }
        if(containsConnectors == true)
        {
            command_lists[counter] = NULL;
            //puts a null at the end of a command so that it wouldn't give 
            //a "no such file or directory" error
            currentConnector = tempString; //says which connector it is

        }
        else if(containsConnectors == false)
        {
            command_lists[counter] = current_token;
            //if no connector,  push token with arguments
        }
        
        counter++;
        current_token = strtok(NULL," "); // goes to next token
    } 
    if(containsConnectors == false)
    {
        command_lists[counter] = NULL;
        //adds NULL so that execvp works correctly
    }
    return currentConnector;

}

void run_command(char** command_list)
{ 
    //runs commaands    
    pid_t pid = fork();
    
    if(pid ==-1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
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
    return;
}

void run_command_with_connectors(char**& final_command,char* command_chara)
{
        
        char* tempa_token = strtok(command_chara," ");
        bool did_it_work = false;// checks if last command succeeded 
        while(tempa_token != NULL)
        {
           string current_connect = connector_parse(final_command,tempa_token);
           //parses input
           int nextExecute = current_connect.find(";");
           int WorkExecute = current_connect.find("&&");
           int failExecute = current_connect.find("||");
           if(exit_check(final_command) == true)
           {
                exit(1);
           }
           //next if statement chain checks for the earliest connector
           if(nextExecute < WorkExecute && nextExecute < failExecute)
           {
               // ; is earliest connector
               if(did_it_work == true)
               {
                    run_command(final_command);
               }
               did_it_work = true;
           }
           else if(WorkExecute < nextExecute && WorkExecute < failExecute)
           {
               //&& is earliest connector
               if(did_it_work == false)
               {
                   did_it_work = false;
               }
               else
               {
                    run_command(final_command);
                    did_it_work = true;
               }
           }
           else if (failExecute < nextExecute && failExecute < WorkExecute)
           {
               //|| is earliest connector
                if(did_it_work == true)
                {
                    //do nothing
                }
                else
                {
                    run_command(final_command);
                    did_it_work = true;
                }
           }
           else
           {
                run_command(final_command);
                //runs command
           }
        }
        return;
	
}

int main(int argc, char**argv) { 

	while(1)//endless loop so that it mimics terminal
	{
		displayPrompt();
		string command_input;
		getline(cin,command_input);	//takes in input
        separator_parser(command_input);  //puts spaces between everything

		char* command_char = new char[command_input.size()];
		strcpy(command_char,command_input.c_str());
        //converts strings into chars
        char** finalist_command = (char**)malloc(BUFSIZ);
        run_command_with_connectors(finalist_command,command_char);
        //attempts to run commands with connectors but is broken right now
        //it runs everything, ignoring connectors
    }
	return 0;
}
