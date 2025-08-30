#include<iostream>
#include"header.h"
#include<queue>
#include<sys/utsname.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<unistd.h>
using namespace std;
string system_name;
queue<string>command_split;
queue<string>pipe_split;
queue<string>space_split;
string Home;

string curr_dir(){
    char cwd[256];
    return string(getcwd(cwd,256));
}

void clear_queue(){
    while(!space_split.empty()){
        space_split.pop();
    }
}

void handle_echo(){
    space_split.pop();
    while(!space_split.empty()){
        cout<<space_split.front()<<" ";
        space_split.pop();
    }
    cout<<endl;
}

void handle_pwd(){
    if(space_split.size()>1){
        while(!space_split.empty()){
            space_split.pop();
        }
        cout<<"invalid command"<<endl;
        return;
    }
    space_split.pop();
    char current_directory[256];
    if(getcwd(current_directory,256)==nullptr){
        cout<<"current directory not fetched"<<endl;
    }
    cout<<current_directory<<endl;
    cout<<flush;
}

void print_prompt()
{
    char *u_name = getenv("USER");
    if (u_name == nullptr)
    {
        cout << "User name not found" << endl;
    }
    string str;
    char cwd[256];
    if(string(getcwd(cwd,256))==Home){
        str="~";
    }
    else{
        str="";
        string path=string(getcwd(cwd,256));
        while(!path.empty()&& path.back()!='/'){
            str+=path.back();
            path.pop_back();
        }
        reverse(str.begin(),str.end());
        str+=">";
    }
    char host_name[256];
    int x = gethostname(host_name, 256);
    if (x == -1)
    {
        cout << "User name can't be fetched" << endl;
        return;
    }
    system_name=(string)u_name+"@"+host_name+":"+str;
}

void handle_cd(){
    if(space_split.size()>2){
        cout<<"Invalid arguments"<<endl;
    }
    else if(space_split.size()==1){
        if(chdir(Home.c_str())!=0){
            perror("cant reach home directory");
        }
    }
    else{
        space_split.pop();
        string flag=space_split.front();
        if(flag==".."){
            if(chdir("..")!=0){
                perror("chdir not executed");
            }
        }
        else if(flag=="~"){
            if(chdir(Home.c_str())!=0){
                perror("cant reach home directory");
            }
        }
        else if(flag=="-"){
            if(chdir("..")!=0){
                perror("chdir not executed");
            }
            cout<<curr_dir()<<endl;
        }
        else{
            if(chdir(flag.c_str())!=0){
                perror("invalid file path");
            }   
        }
    }
    clear_queue();
}

void parser(char *input){
    // Semi colon seperated
    char* input_copy=new char[strlen(input)+1];
    strcpy(input_copy,input);
    const char* delimiter=";";
    char* token=strtok(input_copy,delimiter);
    while(token!=nullptr){
        command_split.push(string(token));
        token=strtok(nullptr,delimiter);
    }
    delete[]input_copy;
    // Pipe seperated
    const char* delimiter2="|";
    while(!command_split.empty()){
        string s=command_split.front();
        command_split.pop();
        char* command=new char[s.length()+1];
        strcpy(command,s.c_str());
        char* token2=strtok(command,delimiter2);
        while(token2!=nullptr){
            pipe_split.push(string(token2));
            token2=strtok(nullptr,delimiter2);
        }
        delete[]command;
        // Space seperated
        const char* delimiter3=" \t";
        while(!pipe_split.empty()){
            string s2=pipe_split.front();
            pipe_split.pop();
            char* command1=new char[s2.length()+1];
            strcpy(command1,s2.c_str());
            char* token3=strtok(command1,delimiter3);
            while(token3!=nullptr){
                space_split.push(string(token3));
                token3=strtok(nullptr,delimiter3);
            }
            // To run each command handle
            while(!space_split.empty()){
                if(space_split.front()=="pwd"){
                    handle_pwd();
                    break;    
                }
                if(space_split.front()=="echo"){
                    handle_echo();
                    break;
                }
                if(space_split.front()=="cd"){
                    handle_cd();
                    break;
                }
                if(space_split.front()=="ls"){
                    handle_ls();
                    clear_queue();
                    break;
                }
                else{
                    cout<<"Invalid command"<<endl;
                    clear_queue();
                }
            }
            delete[] command1;
        }
    }    
}

void get_input(){
    rl_bind_key('\t', rl_insert);
    char *input=readline(system_name.c_str());
    if(!input) return;
    if(*input) add_history(input);
    parser(input);
    free (input);
}

int main(){
    char cwd[256];
    Home=string(getcwd(cwd,256)); 
    while(true){
        print_prompt();
        get_input();     
    }     
    return 0;
}
