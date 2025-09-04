# POSIX SHELL:
The goal of this assignment is to build a custom interactive shell in C++ that accepts user commands, creates and manages new processes, and performs basic functions similar to a Unix shell.

## Files included:

### Header.h 
This file include all the necessary library and the functions which is accessed across the file and is necessary for implementation.
### posix.cpp
This is the main file from where execution of the shell begin and all the other related function called from here.In this file implementation of cd, echo, pwd and pinfo command are done.
### ls.cpp
This file contain the implementation logic of ls command and all its flag.
### bg_fg.cpp
In this file the logic of handling background and foreground process is implemented.
### search.cpp
This file include the implementation of search command and history command.
### pipe.cpp
The logic of executing pipe seperating command is implemented.
### io.cpp
IO redirection and redirection with pipeline is implemented in this file.
### makefile
This file has the compilation logic of all the file at once.
## To start and run the shell:
compile: make   
To run:  ./a.out

