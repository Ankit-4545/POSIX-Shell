#include<iostream>
#include<sys/utsname.h>
#include<unistd.h>
#include<cstring>
#include<stdlib.h>
using namespace std;

int main(){
    while(true){
        char *u_name=getenv("USER");
        if(u_name==nullptr){
            cout<<"User name not found"<<endl;
        }
        char cwd[256];
        if(getcwd(cwd,256)==nullptr){
            cout<<"cwd not executed"<<endl;
            break;
        } 
        char host_name[256];
        int x=gethostname(host_name,256);
        if(x==-1){
            cout<<"User name can't be fetched"<<endl;
            break;
        }
        char input[256];
        cout<<u_name<<"@"<<host_name<<":"<<cwd<<">";
        cin.getline(input,256);
        const char *delimiters="; ";
        char* token=strtok(input,delimiters);
        while(token!=nullptr){
            if(strcmp(token,"exit")==0){
                exit(0);
            }
        }
    }
    return 0;
}
