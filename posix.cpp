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
string Home;

string curr_dir(){
    char cwd[256];
    return string(getcwd(cwd,256));
}

void handle_echo(queue<string>space_split){
    space_split.pop();
    while(!space_split.empty()){
        cout<<space_split.front()<<" ";
        space_split.pop();
    }
    cout<<endl;
}

void handle_pwd(queue<string>space_split){
    if(space_split.size()>1){
        cout<<"invalid command"<<endl;
        return;
    }
    char current_directory[256];
    if(getcwd(current_directory,256)==nullptr){
        cout<<"current directory not fetched"<<endl;
    }
    cout<<current_directory<<endl;
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

void handle_cd(queue<string>space_split){
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
            const char* home = getenv("HOME");
            if(home==nullptr){
                perror("Home is not fetched");
                // clear_queue();
                return;
            }
            if(chdir(home)!=0){
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
    // clear_queue();
}

void execute_command(queue<queue<string>> pipeline_args, bool background) {
    while (!pipeline_args.empty()) {
        queue<string>argument= pipeline_args.front();
        pipeline_args.pop();

        if (argument.empty()) continue;

        string cmd = argument.front();

        if (cmd == "pwd") {
            handle_pwd(argument);
        }
        else if (cmd == "cd") {
            handle_cd(argument);
        }
        else if (cmd == "echo") {
            handle_echo(argument);
        }
        else if (cmd == "ls") {
            handle_ls(argument);
        }
        else if (cmd == "search") {
            handle_search(argument);
        }
        else {
            cout << "Invalid command " << cmd << endl;
        }
    }
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

        bool background = false;
        if (!s.empty() && s.back() == '&') {
            background = true;
            s.pop_back(); 
            while (!s.empty() && isspace(s.back())){ 
                s.pop_back();
            }
        }

        char* command=new char[s.length()+1];
        strcpy(command,s.c_str());
        char* token2=strtok(command,delimiter2);
        while(token2!=nullptr){
            pipe_split.push(string(token2));
            token2=strtok(nullptr,delimiter2);
        }
        delete[]command;

        queue<queue<string>> pipe_argument;
        // Space seperated
        const char* delimiter3=" \t";
        while(!pipe_split.empty()){
            string s2=pipe_split.front();
            pipe_split.pop();
            char* command1=new char[s2.length()+1];
            strcpy(command1,s2.c_str());
            char* token3=strtok(command1,delimiter3);

            queue<string>space;
            string input_file = "";
            string output_file = "";
            bool append_mode = false;

            while(token3!=nullptr){
                string temp=string(token3);
                if (temp == "<") {
                    token3 = strtok(nullptr, delimiter3);
                    if (token3) input_file = string(token3);
                }
                else if (temp == ">") {
                    token3 = strtok(nullptr, delimiter3);
                    if (token3) {
                        output_file = string(token3);
                        append_mode = false;
                    }
                }
                else if (temp == ">>") {
                    token3 = strtok(nullptr, delimiter3);
                    if (token3) {
                        output_file = string(token3);
                        append_mode = true;
                    }
                }
                else {
                    space.push(temp);
                }
                token3 = strtok(nullptr, delimiter3);
            }
            pipe_argument.push(space);

            // // *** CHANGED: Temporary print redirection info (for testing) ***
            // if (!input_file.empty()) {
            //     cout << "Input redirection: " << input_file << endl;
            // }
            // if (!output_file.empty()) {
            //     cout << "Output redirection: " << output_file<< (append_mode ? " (append mode)" : " (overwrite mode)") << endl;
            // }
            // // To run each command handle
            // while(!space_split.empty()){
            //     if(space_split.front()=="pwd"){
            //         handle_pwd();
            //         break;    
            //     }
            //     if(space_split.front()=="echo"){
            //         handle_echo();
            //         break;
            //     }
            //     if(space_split.front()=="cd"){
            //         handle_cd();
            //         break;
            //     }
            //     if(space_split.front()=="ls"){
            //         handle_ls();
            //         clear_queue();
            //         break;
            //     }
            //     if(space_split.front()=="search"){
            //         handle_search();
            //         clear_queue();
            //     }
            //     else{
            //         cout<<"Invalid command"<<endl;
            //         clear_queue();
            //     }
            // }
            delete[] command1;
        }
        execute_command(pipe_argument, background);
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
