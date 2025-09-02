#include"header.h"
#include<iostream>
using namespace std;

void io_redirection(queue<string>command,string input,string output,bool append){
    int pid=fork();
    if(pid<0){
        perror("fork not executed");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        if(!input.empty()){
            int fd=open(input.c_str(),O_RDONLY);
            if(fd==-1){
                perror("file not opened");
                exit(EXIT_FAILURE);
            }
            if(dup2(fd,STDIN_FILENO)==-1){
                perror("file cant be opened");
                close(fd);
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        if(!output.empty()){
            int flag;
            if(append==true){
                flag=O_WRONLY|O_CREAT|O_APPEND;
            }
            else{
                flag=O_WRONLY|O_CREAT|O_TRUNC;
            }
            int fd=open(output.c_str(),flag,0644);
            if(fd==-1){
                perror("file not opened");
                exit(EXIT_FAILURE);
            }
            if(dup2(fd,STDOUT_FILENO)==-1){
                perror("file cant be opened");
                close(fd);
                exit(EXIT_FAILURE);    
            }
            close(fd);
        }
        int n=command.size();
        char** arg=new char*[n+1];
        int x=0;
        while(!command.empty()){
            arg[x]=strdup(command.front().c_str());
            x++;
            command.pop();
        }
        arg[n]=nullptr;
        execvp(arg[0],arg);
        perror("execvp failed to read");
        exit(EXIT_FAILURE);
    }
    else{
        int stat;
        waitpid(pid,&stat,0);
    }
}