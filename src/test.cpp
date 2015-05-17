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
int std_in;
int std_out;

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
    //int counter = 0;
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
	//cout << "position: " << position <<endl;
	// << ";" << NextFind << endl;
	//cout << "&&" << ifWorkFind << endl;
	//cout << "arrow: " << outputFind << "      " << appendFind << endl;
	//cout << "stick: " << pipeFind << "      " << ifFailFind << endl;
	if(NextFind != -1 
	&& (NextFind < ifWorkFind || ifWorkFind == -1 ) 
	&& (NextFind < ifFailFind || ifFailFind == -1) 
	&& (NextFind < appendFind || appendFind == -1) 
	&& (NextFind < inputFind || inputFind == -1)
	&& (NextFind < outputFind || outputFind == -1) 
	&& (NextFind < pipeFind || pipeFind == -1) )
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
	    && (inputFind < NextFind || NextFind == -1)
	    && (inputFind < outputFind || outputFind == -1) 
	    && (inputFind < pipeFind || pipeFind == -1) )
            {
          //     cout << ":<" << endl; 
                //adds spaces around > connector
                string_input.insert(inputFind, " ");
                string_input.insert(string_input.find("<",position)+1," ");
                my_iterator = string_input.begin() + string_input.find("<")+1;
                position = string_input.find("<", position)+1;
	//	cout << "pos: " << position << endl;
            }
        else if(ifWorkFind != -1 
	    && (ifWorkFind < NextFind || NextFind == -1 ) 
	    && (ifWorkFind < ifFailFind || ifFailFind == -1) 
	    && (ifWorkFind < appendFind || appendFind == -1) 
	    && (ifWorkFind < inputFind || inputFind == -1)
	    && (ifWorkFind < outputFind || outputFind == -1) 
	    && (ifWorkFind < pipeFind || pipeFind == -1) )
        {
            //cout << ":&&" << endl;
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
	    && (appendFind < inputFind || inputFind == -1)
	    && (appendFind <= outputFind || outputFind == -1) 
	    && (appendFind < pipeFind || pipeFind == -1))
        {
          //  cout << ":>>" << endl;
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
	    && (ifFailFind < inputFind || inputFind == -1)
	    && (ifFailFind < outputFind || outputFind == -1) 
	    && (ifFailFind <= pipeFind || pipeFind == -1))
        {
        //	cout << position << ":||" << endl;
            //adds spaces around || connector
            string_input.insert(ifFailFind, " ");
            string_input.insert(string_input.find("||",position)+2," ");
            my_iterator = string_input.begin() + string_input.find("||")+2;
            position = string_input.find("||", position)+2;
	    //cout << "pos2: " << position << endl;
        }
	else if(outputFind != -1 
	    && (outputFind < NextFind || NextFind == -1 ) 
	    && (outputFind <= appendFind || appendFind == -1) 
	    && (outputFind < ifWorkFind || ifWorkFind == -1) 
	    && (outputFind < inputFind || inputFind == -1)
	    && (outputFind < ifFailFind || ifFailFind == -1) 
	    && (outputFind < pipeFind || pipeFind == -1))
        {
        //	cout << ":>" << endl;
            //adds spaces around || connector
            string_input.insert(outputFind, " ");
            string_input.insert(string_input.find(">",position)+1," ");
            my_iterator = string_input.begin() + string_input.find(">",position)+1;
            position = string_input.find(">", position)+1;
        }
	else if(pipeFind != -1 
	    && (pipeFind < NextFind || NextFind == -1 ) 
	    && (pipeFind < appendFind || appendFind == -1) 
	    && (pipeFind < ifWorkFind || ifWorkFind == -1) 
	    && (pipeFind < inputFind || inputFind == -1)
	    && (pipeFind <= ifFailFind || ifFailFind == -1) 
	    && (pipeFind < outputFind || outputFind == -1))
        {
        //	cout << ":|" << endl;
            //adds spaces around | connector
            string_input.insert(pipeFind, " ");
            string_input.insert(string_input.find("|",position)+1," ");
            my_iterator = string_input.begin() + string_input.find("|",position)+1;
            position = string_input.find("|", position)+1;
        }
	//counter++;
	//cout << string_input << endl;
	//cout << "position2: " << position <<endl;
	//cout << ";" << NextFind << endl;
	//cout << "&&" << ifWorkFind << endl;
	//cout << "arrow2: " << outputFind << "      " << appendFind << endl;
	//cout << "stick2: " << pipeFind << "      " << ifFailFind << endl;
	//cout << counter << endl;
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
    //cout << "ran: " << command_list[0] << endl;
    //cout << command_list[1] << endl;
    pid_t pid = fork();
    int worked = 0;
    //cout << "running" << endl;
    if(pid ==-1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
    	worked = execvp(command_list[0],command_list);
        //cout << "command:  " << command_list[0] << endl;
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
string redirection(char** command)
{
	int i = 0;
	string temp;
	string both = "both";
	string out = "out";
	string in = "in";
	string none = "none";
	string fail = "fail";
	bool input = false;
	bool output = false;
	//cout << "lose " << endl;
	//cout << "lost: " << command[0] << endl;
	temp = command[0];
	if(temp == ">>" 
	|| temp == "<"
	|| temp == ">")
	{
	//	cout << "fail" << endl;
		return fail;
	}
	while(command[i] != NULL)
	{
		temp = command[i];
		if(temp == "<")
		{
			input = true;
		}
		else if(temp == ">" || temp == ">>")
		{
			output = true;
		}
		i++;

	}
	//cout << "hmm" << endl;
	if(input == true && output == true)
	{
	//	cout << "a" << endl;
		return both;
	}
	else if(input == false && output == true)
	{
	//	cout << "b" << endl;
		return out;
	}
	else if(input == true && output == false)
	{
	//	cout << "c" << endl;
		return in;
	}
	else
	{
	//	cout << "d" << endl;
		return none;
	}
}
bool input_output(char** command)
{
	string temp;
	string inputTemp;
	string outputTemp;
	string last_out;;
	bool stop;
	int end = 0;
	
	bool first = true;
	int i = 0;
	int last_input = 0;
	int last_output = 0;
	string last_redirect;
	int finale_position = 1;
       	char** finale = (char**)malloc(BUFSIZ);
	while(command[i] != NULL)
	{
		//cout << command[i] << endl;
		temp = command[i];
		if(temp == "<"
//		&& (i < find_append || find_append == -1)
//		&& (i < find_output || find_append == -1)
)
		{
			last_redirect = "<";
			last_input = i;
			//finale[1] = command[i];
			//input_redirect = true;
			if(command[i+1] != NULL)
			{
				inputTemp = command[i+1];
			}
		}
		else if(temp == ">")
		{
			//cout << "1" << endl;
			if(first)
			{
				end = i;
				first = false;
			}
			last_out = temp;	
			last_output = i;
			if(command[i+1] != NULL)
			{
				outputTemp = command[i+1];
			//	cout << outputTemp << endl;
			}
			//if(last_redirect == "<")
			//{
			//	break;
			//}
		}
		else if(temp == ">>")
		{
			//cout << "2" << endl;
			if(first)
			{
				end = i;
				first = false;
			}
			last_out = temp;
			last_output = i;
			if(command[i+1] != NULL)
			{
				outputTemp = command[i+1];
			//	cout << "2" << outputTemp << endl;
			}
		}
		else
		{
			//cout << i << ": " << command[i] << endl;
			if(i != 0)
			{
				finale[finale_position] = command[i];
				finale_position++;
			}
			//finale_position++;
			//cout << "fin" << finale[finale_position] << endl;
		}
		//outputTemp = "penis.txt";
		//cout << "lat: " << outputTemp.c_str() << endl;
		//cout << "end: " << outputTemp.size() << endl;
		//temp = command[i];
		i++;
	}
	/*
	if(input_redirect == false)
	{
		return false;
	}
	*/
	

	finale[0] = command[0];
	//finale[1] = command[last_input ];
	//cout << "last: " << last_input << endl;
	finale[last_input+1] = NULL;
	//cout << "input: " << inputTemp << endl;	
	//cout << finale[1] << endl;
	//cout << "output: " << outputTemp << endl;	
	int ID;
	int READ;
	if((READ = open(inputTemp.c_str(),O_RDONLY)) == -1)
	{
		perror("open");
	}
	if(last_out == ">")
	{
		if((ID = open(outputTemp.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
		{
			perror("open");
		}
	}
	else if(last_out == ">>")
	{

		if((ID = open(outputTemp.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0664)) == -1)
		{
			perror("open");
		}
	}
	if(dup2(READ,0) ==-1)
	{
		perror("dup");
		_exit(1);
	}
	if(dup2(ID,1) == -1)
	{
		perror("dup2");
		_exit(1);
	}
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork");
		_exit(1);
	}
	else if(pid == 0)
	{
		if((execvp(finale[0],finale)) == -1)
		{
			perror("execvp");
		}
		_exit(1);
	}
	else if(pid > 0)
	{
		if(close(READ) ==-1)
		{
			perror("close");
			_exit(1);
		}
		if(close(ID) == -1)
		{
			perror("close");
			_exit(1);
		}
		if(wait(0) == -1)
		{
			perror("wait");
			_exit(1);
		}
		if(dup2(std_in,0) == -1)
		{
			perror("dup2");
			_exit(1);
		}
		if(dup2(std_out,1) == -1)
		{
			perror("dup2");
			_exit(1);
		}
	}
	free(finale);
	return true;
}
bool output_redirection(char** command)
{
/*
dup2(std_in, 0);
	cout << std_in << "    " << std_out << endl;
	dup2(std_out,1);
	int fd[2];
	pipe(fd);
	int id = -1;
	int i = 0;
	*/
	bool output_redirect = false;
	string temp = command[0];
	int last_output = 0;
	string last_out;
	int i = 0;
	int id;
	bool first = true;
	int end = 0;
	char** finale = (char**)malloc(BUFSIZ);
	while(command[i] != NULL)
	{
		//cout << command[i] << endl;
		if(temp == ">"
	//	&& (i < find_append || find_append == -1)
//		&& (i < find_output || find_append == -1)
)
		{
			if(first)
			{
				end = i;
				first = false;
			}
			last_output = i;
			//finale[1] = command[i];
			output_redirect = true;
			last_out = ">";
		}
		else if(temp == ">>"
	//	&& (i < find_output || find_output == -1)
		)
		{
			if(first)
			{
				end = i;
				first = false;
			}
			last_output = i;
			output_redirect = true;
			last_out = ">>";
		}
		else
		{
		//	cout << i << endl;
			finale[i] = command[i];
		}
		//cout << i + 100 << endl;
		//cout << command[i] << endl;
		
		temp = command[i];
		i++;
	}
	if(output_redirect == false)
	{
		return false;
	}
	temp = command[last_output];
	//cout << temp << endl;
	finale[end -1] = NULL;
	if(last_out == ">")
	{
		if((id = open(temp.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
		{
			perror("open");
			_exit(1);
		}
	}
	else if(last_out == ">>")
	{

		if((id = open(temp.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0664)) == -1)
		{
			perror("open");
			_exit(1);
		}
	}
	int pid = fork();
	if(pid == 0)
	{
		
		if(close(1) == -1)
		{
			perror("close");
		}
		if(dup(id) == -1)
		{
			perror("dup");
			_exit(1);
		}
		if(execvp(finale[0],finale) == -1)
		{
			perror("execvp");
		}
		_exit(1);
	}
	else if(pid > 0)
	{
		if(wait(0) == -1)
		{
			perror("wait");
			_exit(1);
		}
		if(close(id) == -1)
		{
			perror("close");
			_exit(1);
		}
	}
	free(finale);
	return true;
}
bool input_redirection(char** command)
{
	int i = 0;
	//cout << "af: " << command << endl;
	string temp;
	string filed;
	bool input_redirect = false;
	int last_input = 0;
	bool chained_input = false;
	bool chained_append = false;
	bool chained_output = false;
	while(command[i] != NULL)
	{
		string tempurary = command[i];
		if(tempurary == ">>")
		{
//			find_append = i;
		}
		else if(tempurary == ">")
		{
//			find_output = i;
		}
		i++;
	}
	i = 0;
	while(command[i] != NULL)
	{
		//cout << command[i] << endl;
		if(temp == "<"
//		&& (i < find_append || find_append == -1)
//		&& (i < find_output || find_append == -1)
)
		{
			last_input = i;
			//finale[1] = command[i];
			input_redirect = true;
		}
		temp = command[i];
		i++;
	}
	if(input_redirect == false)
	{
		return false;
	}
	
	
       	char** finale = (char**)malloc(BUFSIZ);

	finale[0] = command[0];
	finale[1] = command[last_input];
	finale[2] = NULL;
	//cout << command[i-1];
	int pid;
	int FileID;
	if((FileID = open(temp.c_str(), O_RDONLY)) == -1)
		{
			perror("open");
			_exit(1);
		}
	//int IDhold;
	
//	int fd;
	
	pid = fork();
	if(pid == -1)
	{
		perror("fork()");
		_exit(1);
	}
	else if(pid == 0)
	{
		if(close(0) == -1)
		{
			perror("close");
			_exit(1);
		}
		if(dup(FileID) == -1)
		{
			perror("dup");
			_exit(1);
		}
		if((execvp(finale[0],finale)) == -1)
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
		if(close(FileID) == -1)
		{
			perror("wait");
			_exit(1);
		}
	}
	free(finale);
	return true;
	
}

void piping()
{

	return;
}
void run_command_with_connectors(char**& final_command,char* command_chara)
{
        string nextGo = ";";
	string workGo = "&&";
	string failGo = "||";
	char* tempa_token = strtok(command_chara," ");
        bool did_it_work = true;//checks if last command succeeded 
	//true if not supposed to run through the chain
	bool chained_or = false;
	bool chained_and = false;

	string current_connect = ";";
	while(tempa_token != NULL)
        {
		//cout << "toucan: " << tempa_token << endl;
		//cout << "current :" << current_connect << endl;
		current_connect = connector_parse(final_command,tempa_token);
		//parses input
		int nextExecute = current_connect.compare(nextGo);
		int workExecute = current_connect.compare(workGo);
		int failExecute = current_connect.compare(failGo);
		int sized = 0;
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
				//cout << "red" << endl;
				string red  = redirection(final_command);
				//cout << red << endl;
				//cout << "blue" << endl;
				if(red == "fail")
				{
					cerr << "error: passed in io as first argument" << endl;
					_exit(1);
				}
				else if(red == "in")	
				{ 
					//cerr << "in" << endl;
					input_redirection(final_command);
				}
				else if(red == "out")
				{
					//cerr << "out" << endl;
					output_redirection(final_command);

				}
				else if(red == "both")
				{
					//cerr << " both"  << endl;
					input_output(final_command);
				}
				else if(red == "none")
				{
					//cerr << "none" << endl;
					run_command(final_command,did_it_work);
				}
				
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
	//			cout << "ayy :";
	//			cout << redirection_parse(final_command) << endl;

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
				//cout << "ayy :";
				//cout << redirection_parse(final_command)<< endl;
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
/*
fix command execution ( if statements on when to run)
piping
readme
*/
int main(int argc, char**argv) { 

	std_in = dup(0);
	std_out = dup(1);
	//cout << std_in << "    " << std_out << endl;
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
        	//cout << command_char << endl;
		//converts strings into chars
       		 char** finalist_command = (char**)malloc(BUFSIZ);
		run_command_with_connectors(finalist_command,command_char);
        	delete[] command_char;
        	free(finalist_command);
		if(dup2(std_in, 0) == -1)
		{
			perror("dup2");
			_exit(1);
		}
		if(dup2(std_out,1) == -1)
		{
			perror("dup2");
			_exit(1);
		}
    	}
	return 0;
}
