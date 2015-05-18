#rshell

Rshell is a custom basic command shell used to mimic the BASH terminal. Rshell is able to run single commands. Rshell is able to run commands that use connectors such as ";" which executes the next command always,"&&" which executes the next command if the previous command succeeded, and "||" which executes the next command if the previous command failed.

It can also do input redirection, output redirection, and piping. Piping however only works once.

#ls
This is my custom version of ls. There is no color coding nor output management. 
It outputs vertically in one column. 

The only flags enabled are -a,-l and its combinations such as -la or -al.

Inputs passed in without '-' preceding it will be considered a directory or path in which to run the ls command.


##Installation
```
$git clone https://github.com/amah001/rshell.git
$cd rshell
$git checkout hw2
$make
$bin/rshell
$bin/ls
```
##Bugs/ Limitations for rshell
```
piping does not work past one pipe
input and output redirection only do the last of their respective chains
ex: only do d
    cat < a < b < c < d 
        means: cat d
    echo world > a >b > c > d
        means: world into d

Edge cases do not work.
```
##Bugs/Limitations for ls
```
-R doesn't work at all.
There is no color coding.
Output is one vertical column.
Edge cases do not work.
only works on -l and -a flags
doesn't work for individual files eg. "ls a.out"

```
##License
[License is in this link.](/LICENSE)
