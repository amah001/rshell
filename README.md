#rshell

Rshell is a custom basic command shell used to mimic the BASH terminal. Rshell is able to run single commands. Rshell is supposed to be able to run commands that use connectors such as ";" which executes the next command always,"&&" which executes the next command if the previous command succeeded, and "||" which executes the next command if the previous command failed.

##Installation
```
$git clone https://github.com/amah001/rshell.git
$cd rshell
$git checkout hw0
$make
$bin/rshell
```
##Bugs
```
Commands will run regardless of connectors
```
##License
