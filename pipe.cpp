#include"header.h"
#include<iostream>
using namespace std;

void handle_inbuilt(queue<string>argument){
    if(argument.front()=="ls")handle_ls(argument);
    if(argument.front()=="cd")handle_cd(argument);
    if(argument.front()=="echo")handle_echo(argument);
    if(argument.front()=="search")handle_search(argument);
    if(argument.front()=="pwd")handle_pwd(argument);
}

void execute_pipe(deque<trackbg>pipe_argument){
    int n=pipe_argument.size();
    int size=2*(n-1); 
    int pipefd[size];
    for(int i=0;i<n-1;++i){
        if(i!=n-1){
            if(pipe_argument[i].bg){
                cout<<"Invalid argument"<<endl;
                return;
            }
        }
    }   
    for(int i=0;i<n-1;++i){
        if(pipe(&pipefd[2*i])==-1){
            perror("Pipe not executed");
            return;
        }
    }
    for(int i=0;i<n;++i){
        queue<string>argument=pipe_argument[i].arg;
        int pid=fork();
        if(pid==-1){
            perror("Child propcess not executed");
            exit(EXIT_FAILURE);
        }
        if(pid==0){
            if(i==0){
                if(dup2(pipefd[1],STDOUT_FILENO)==-1){
                    perror("dup2 not executed");
                    exit(EXIT_FAILURE);;
                }
            }
            else if(i==n-1){
                if(dup2(pipefd[2*(i-1)],STDIN_FILENO)==-1){
                    perror("dup2 not executed");
                    exit(EXIT_FAILURE);;
                }
            }
            else{
                if(dup2(pipefd[2*(i-1)],STDIN_FILENO)==-1){
                    perror("dup2 not executed");
                    exit(EXIT_FAILURE);;
                }
                if(dup2(pipefd[2*i+1],STDOUT_FILENO)==-1){
                    perror("dup2 not executed");
                    exit(EXIT_FAILURE);;
                }
            }
            for(int j=0;j<size;++j){
                close(pipefd[j]);
            }
            // io handling
            if(!pipe_argument[i].input.empty() || !pipe_argument[i].output.empty()){
                io_redirection(pipe_argument[i].arg,pipe_argument[i].input,pipe_argument[i].output,pipe_argument[i].append);
                exit(EXIT_SUCCESS);
            }
            if(!custom(argument.front())){
                handle_inbuilt(argument);
                exit(EXIT_SUCCESS);
            }
            else{
                char** temp=new char *[argument.size()+1];
                int x=0;
                while(!argument.empty()){
                    temp[x]=strdup(argument.front().c_str());
                    cout<<temp[x]<<" ";
                    x++;
                    argument.pop();
                }
                temp[x]=nullptr;
                if(execvp(temp[0],temp)==-1){
                    perror("execvp not executed");
                    for(int k=0;k<x;++k){
                        free(temp[k]);
                    }
                    delete[]temp;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    for(int i=0;i<size;++i){
        close(pipefd[i]);
    }
    for(int i=0;i<n;++i){
        int stat;
        wait(&stat);
    }
}