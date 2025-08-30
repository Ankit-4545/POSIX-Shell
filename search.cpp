#include<iostream>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include"header.h"
using namespace std;

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
        string comp_path = curr + "/" + entry->d_name;
        int x=stat(comp_path.c_str(),&buf);
        if(x==0 && S_ISDIR(buf.st_mode)){
            if(search(comp_path,file)){
                return true;
            }
        }
    }
    closedir(dir);
    return false;
}

void handle_search(){
    char cwd[256];
    string current=string(getcwd(cwd,256));
    if(space_split.size()>2){
        cout<<"Invalid arguments"<<endl;
        return;
    }
    space_split.pop();
    bool res=search(current,space_split.front());
    if(res) cout<<"true"<<endl;
    else cout<<"false"<<endl;
}