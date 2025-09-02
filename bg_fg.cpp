#include<iostream>
#include"header.h"
#include<unistd.h>
using namespace std;

int custom(string command){
    if(command=="pwd")return 0;
    if(command=="echo")return 0;
    if(command=="ls")return 0;
    if(command=="cd")return 0;
    if(command=="search")return 0;
    if(command=="pinfo")return 0;
    return 1;
}

void handle_foreground(queue<string>argument){
    vector<char*>arg;
    while(!argument.empty()){
        string args=argument.front();
        argument.pop();
        arg.push_back(strdup(args.c_str()));
    }
    arg.push_back(nullptr);
    int pid=fork();
    if(pid<0){
        perror("fork execution failed");
        for(char* c:arg)free(c);
        return;
    }
    else if(pid==0){
        if(execvp(arg[0],arg.data())==-1){
            perror("execvp execution failed");
            return;
        }
    }
    else{
        int status;
        waitpid(pid,&status,0);
    }
    for(int i=0;i<arg.size();++i){
        free(arg[i]);
    }
}

void handle_background(queue<string>argument){
    vector<char*>arg;
    while(!argument.empty()){
        string args=argument.front();
        argument.pop();
        arg.push_back(strdup(args.c_str()));
    }
    arg.push_back(nullptr);
    int pid=fork();
    if(pid<0){
        perror("fork execution failed");
        for(char* c:arg)free(c);
        return;
    }
    else if(pid==0){
        if(execvp(arg[0],arg.data())==-1){
            perror("execvp execution failed");
            return;
        }
    }
    else{
        cout<<pid<<endl;
    }
    for(int i=0;i<arg.size();++i){
        free(arg[i]);
    }
}