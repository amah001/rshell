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
#include <fcntl.h>

using namespace std;

void displayPrompt()
{   
    // shows username and hostname
    // BUFSIZ = BUFFERSIZE
	char login[BUFSIZ];
	if((getlogin_r(login,BUFSIZ)) != 0)
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
    
	cout << login << "@" << host  << ":$";
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
    int counter = 0;
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
	int appendFind = string_input.find(">>", position);
		//looks for >>
	int inputFind = string_input.find("<",position);
		//looks for <
	int outputFind = string_input.find(">",position);
		//looks for >
	int pipeFind = string_input.find("|",position);
		//looks for |
	if(NextFind != -1 
	&& (NextFind < ifWorkFind || ifWorkFind == -1 ) 
	&& (NextFind < ifFailFind || ifFailFind == -1) 
	&& (NextFind < appendFind || appendFind == -1) 
	&& (NextFind < inputFind || inputFind == -1))
            {
                
                //adds spaces around ; connector
                string_input.insert(NextFind, " ");
                string_input.insert(string_input.find(";",position)+1," ");
                my_iterator = string_input.begin() + string_input.find(";")+1;
                position = string_input.find(";", position)+1;
            }
	else if(inputFind != -1 
	    && (inputFind < ifWorkFind || ifWorkFind == -1 ) 
	    && (inputFind < ifFailFind || ifFailFind == -1) 
	    && (inputFind < appendFind || appendFind == -1) 
	    && (inputFind < NextFind || NextFind == -1))
            {
                
                //adds spaces around > connector
                string_input.insert(inputFind, " ");
                string_input.insert(string_input.find("<",position)+1," ");
                my_iterator = string_input.begin() + string_input.find("<")+1;
                position = string_input.find("<", position)+1;
            }
        else if(ifWorkFind != -1 
	    && (ifWorkFind < NextFind || NextFind == -1 ) 
	    && (ifWorkFind < ifFailFind || ifFailFind == -1) 
	    && (ifWorkFind < appendFind || appendFind == -1) 
	    && (ifWorkFind < inputFind || inputFind == -1))
        {
            cout << "welp" << endl;
            //adds spaces around && connector
            string_input.insert(ifWorkFind, " ");
            string_input.insert(string_input.find("&&",position)+2," ");
            my_iterator = string_input.begin() + string_input.find("&&")+2;
            position = string_input.find("&&", position)+2;
        }
	else if(appendFind != -1 
	    && (appendFind < NextFind || NextFind == -1 ) 
	    && (appendFind < ifFailFind || ifFailFind == -1) 
	    && (appendFind < ifWorkFind || ifWorkFind == -1) 
	    && (appendFind < inputFind || inputFind == -1)	   )
        {
            cout << "obi" << endl;
            //adds spaces around >>
            string_input.insert(appendFind, " ");
            string_input.insert(string_input.find(">>",position)+2," ");
            my_iterator = string_input.begin() + string_input.find(">>")+2;
            position = string_input.find(">>", position)+2;
        }
        else if(ifFailFind != -1 
	    && (ifFailFind < NextFind || NextFind == -1 ) 
	    && (ifFailFind < appendFind || appendFind == -1) 
	    && (ifFailFind < ifWorkFind || ifWorkFind == -1) 
	    && (ifFailFind < inputFind || inputFind == -1))
        {
        	cout << "heads" << endl;
            //adds spaces around || connector
            string_input.insert(ifFailFind, " ");
            string_input.insert(string_input.find("||",position)+2," ");
            my_iterator = string_input.begin() + string_input.find("||")+2;
            position = string_input.find("||", position)+2;
        }
	counter++;
	cout << counter << endl;
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
    	currentConnector = ";";
        command_lists[counter] = NULL;
        //adds NULL so that execvp works correctly
    }
    return currentConnector;

}

void run_command(char** command_list, bool &works)
{ 
    //runs commands    
    pid_t pid = fork();
    int worked = 0;
    if(pid ==-1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
    	worked = execvp(command_list[0],command_list);
        if(worked == -1)
        {
	    perror("execvp");
        }
        _exit(1);
    }
    else if(pid > 0)
    {
    	int a = waitpid(pid, &worked,0);
        if(a == -1)
        {
	    perror("wait()");
        }
	if(worked > 0)
	{
		works = false;
	}
	else
	{
		works = true;
	}
    }
    return;
}
string redirection_parse(char** command)
{
	string current_redirect = command[0];
	char** redirectiongroup = (char**)malloc(BUFSIZ);
	
	/*
	bool containRedirection = false;
	string append = ">>";
	string output_redirect = ">";
	string input_redirect = "<";
	string pipe = "|";
	string current_redirect;
	size_t neverFound = -1;
	while(toucanator != NULL && containRedirection == false)
	{
		string tempaStringa = command;
		if(tempaStringa.find("<") != neverFound
			|| tempaStringa.find("<<") != neverFound
			|| tempaStringa.find("|") != neverFound)
		{
			containRedirection = true;
		}
		if(containRedirection == true)
		{

		}
		else if(containRedirection == false)
		{

		}
	}
	if(containRedirection = false)
	{
		//return;
	}
	*/
	return current_redirect;
}
void input_redirection()
{
	int pid;
	int id;
	int fd;
	/*
	pid = fork();
	if(pid == -1)
	{
		perror("fork()");
		_exit(1);
	}
	else if(pid == 0)
	{
		if((execvp(argv[0],argv) == -1)
		{
			perror("execvp()");
		}
		_exit(1);

	}
	else if(pid > 0)
	{
		if(wait(0) == -1)
		{
			perror("wait()");
			_exit(1);
		}
	}
	*/
	return;
	
}

void output_redirection()
{

	return;
}
void piping()
{

	return;
}
void redirection_separator(string& string_input)
{
/*
	string::iterator my_iterator;
	int position = 0;
 	for( my_iterator = string_input.begin();
    	my_iterator < string_input.end();
    	my_iterator++, position++)
    	{
 		int inputFind = string_input.find("<", position);
		int outputFind = string_input.find(">", position);
		int appendFind = string_input.find("> >", position);
		int pipeFind = string_input.find(" | ", position);
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
   */
	return;
}
void run_command_with_connectors(char**& final_command,char* command_chara)
{
        string nextGo = ";";
	string workGo = "&&";
	string failGo = "||";
	cout << command_chara << endl;
	/*
	int findSemi = command_chara.strch(";");
	int findAnd = command_chara.strch("&&");
	int findOr  = command_chara.strch("||");
	*/
	char* tempa_token = strtok(command_chara," ");
	/*
	if(findSemi < findAnd && findSemi < findOr)
	{
		tempa_token = strtok(command_chara,";");
	}
	else if(findAnd < findSemi && findAnd < findOr)
	{
		tempa_token = strtok(command_chara,"&&");
	}
	else if(findOr < findSemi && findOr < findAnd)
	{
		tempa_token = strtok(command_chara,"||");
	}
	*/
        bool did_it_work = true;//checks if last command succeeded 
	//true if not supposed to run through the chain
	bool chained_or = false;
	bool chained_and = false;

	string current_connect = ";";
	while(tempa_token != NULL)
        {
		//cout << tempa_token << endl;
	//	cout << "current:" << current_connect << endl;
		current_connect = connector_parse(final_command,tempa_token);
		//parses input
		int nextExecute = current_connect.compare(nextGo);
		int workExecute = current_connect.compare(workGo);
		int failExecute = current_connect.compare(failGo);
		if(exit_check(final_command) == true)
		{
		     exit(1);
		}
		//next if statement chain checks for the earliest connector
		if(nextExecute == 0)
		{
			//cout << "kilo" << endl;
		// ; is earliest connector
			if(chained_or || chained_and)
			{
				//do nothing
			}
			else 
			{
				//cout << "ayy :";
				//const char** false_command = final_command;
				//cout << redirection_parse(final_command) << endl;
				run_command(final_command,did_it_work);
			}
			chained_or = false;
			chained_and = false;

		}
		else if(workExecute == 0)
		{
		//	cout << "world" << endl;
			//&& is earliest connector
			if(chained_and == true)
			{
			//	cout << "a" << endl;
				chained_and = true;
				//do nothing
			}
			else if(chained_or == true)
			{
			//	cout << "b" << endl;
				chained_or = false;
				chained_and = false;
			}
			else
			{	    
			//	cout << "c" << endl;
				cout << "ayy :";
				cout << redirection_parse(final_command) << endl;

				run_command(final_command,did_it_work);
				if(did_it_work == false)
				{
					//cout << "d" << endl;
					chained_and = true;
				}
			}
		}
		else if (failExecute == 0)
		{
			//cout << "rito" << endl;
			//|| is earliest connector
			if (chained_or == true)
			{
			//	cout << "z" << endl;
				chained_or = true;
			    //do nothing
			}
			else if(chained_and == true)
			{
			//	cout << "g" << endl;
				chained_or = false;
				chained_and = false;
			}
			else
			{
			//cout << "m" << endl;;
				cout << "ayy :";
				cout << redirection_parse(final_command)<< endl;


			    run_command(final_command,did_it_work);
			    if(did_it_work == true)
			    {
		//	    	cout << "f" << endl;
				chained_or = true;
			    }
			}
		}

		else
		{
			cout << "doge" << endl;
		}

	//	current_connect = connector_parse(final_command,tempa_token);
		   
        }
        return;
	
}

int main(int argc, char**argv) { 

	while(1)//endless loop so that it mimics terminal
	{
		displayPrompt();
		string command_input;
		getline(cin,command_input);	//takes in input
	        //command_input = ';' + command_input;
		//cout << command_input << endl;
		separator_parser(command_input);  //puts spaces between everything

		char* command_char = new char[command_input.size()+1];
		strcpy(command_char,command_input.c_str());
        	//converts strings into chars
       		 char** finalist_command = (char**)malloc(BUFSIZ);
		run_command_with_connectors(finalist_command,command_char);
        	delete[] command_char;
        	free(finalist_command);
		//attempts to run commands with connectors but is broken right now
        	//it runs everything, ignoring connectors
           
    }
	return 0;
}
