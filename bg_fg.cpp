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
    if(command=="exit")return 0;
    if(command=="history")return 0;
    return 1;
}

void execute_inbuilt(queue<string>argument){
    string cmd=argument.front();
    if(cmd=="pwd"){
        handle_pwd(argument);
    }
    else if(cmd=="cd"){
        handle_cd(argument);
    }
    else if(cmd=="echo"){
        handle_echo(argument);
    }
    else if(cmd=="ls"){
        handle_ls(argument);
    }
    else if(cmd=="search"){
        handle_search(argument);
    }
    else if(cmd=="pinfo"){
        handle_pinfo(argument);
    }
    else if(cmd=="history"){
        show_history(argument);
    }
    else if(cmd=="exit"){
        exit(0);
    }
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
        // setpgid(0,0);
        if(execvp(arg[0],arg.data())==-1){
            cout<<"Invalid Command"<<endl;
            exit(1);
        }
    }
    else{
        // setpgid(pid,pid);
        fgpid=pid;
        int status;
        waitpid(pid,&status,WUNTRACED);
        fgpid=-1;
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
        setpgid(0,0);
        if(execvp(arg[0],arg.data())==-1){
            perror("execvp execution failed");
            return;
        }
    }
    else{
        cout<<pid<<endl;
        setpgid(pid,pid);
        fgpid=-1;
    }
    for(int i=0;i<arg.size();++i){
        free(arg[i]);
    }
}