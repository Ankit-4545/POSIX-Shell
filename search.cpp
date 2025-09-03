#include<iostream>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include"header.h"
using namespace std;

void show_history(queue<string>argument){
    if(!argument.empty())argument.pop();
    int total;
    if(argument.size()>=2){
        cout<<"Invalid argument"<<endl;
        return;
    }
    if(argument.size()==1){
        total=stoi(argument.front());
        if(total>history_length){
            cout<<"invalid range"<<endl;
            return;
        }
    }
    else{
        total=10;
    }
    int start;
    if(history_length-total==0){
        start=1;
    }
    else{
        start=history_length-total+1;
    }
    int num=0;
    HIST_ENTRY** entry=history_list();
    for (int i = start-1; i < history_length; ++i) {
        if(entry){
            cout<<++num<<": "<<entry[i]->line<<endl;
        }
    }
}

bool search(string curr,string file){
    DIR* dir=opendir(curr.c_str());
    if(dir==nullptr){
        perror("Opendir not executed");
        return false;
    }
    struct dirent* entry;
    struct stat buf;
    while((entry=readdir(dir))!=nullptr){
        if(string(entry->d_name)=="."||string(entry->d_name)==".."){
            continue;
        }
        if(string(entry->d_name)==file){
            closedir(dir);
            return true;
        }
        string current=curr+"/"+entry->d_name;
        int x=stat(current.c_str(),&buf);
        if(x==0 && S_ISDIR(buf.st_mode)){
            if(search(current,file)){
                return true;
            }
        }
    }
    closedir(dir);
    return false;
}

void handle_search(queue<string>argument){
    char cwd[256];
    string current=string(getcwd(cwd,256));
    if(argument.size()>2){
        cout<<"Invalid arguments"<<endl;
        return;
    }
    argument.pop();
    bool res=search(current,argument.front());
    if(res==true)cout<<"true"<<endl;
    else cout<<"false"<<endl;
}