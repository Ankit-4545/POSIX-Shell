#include<iostream>
#include"header.h"
using namespace std;
string system_name;
queue<string>command_split;
string Home;
vector<string>hist;

string curr_dir(){
    char cwd[256];
    return string(getcwd(cwd,256));
}

void handle_echo(queue<string>argument){
    argument.pop();
    while(!argument.empty()){
        cout<<argument.front()<<" ";
        argument.pop();
    }
    cout<<endl;
}

void handle_pwd(queue<string>argument){
    if(argument.size()>1){
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

void handle_cd(queue<string>argument){
    if(argument.size()>2){
        cout<<"Invalid arguments"<<endl;
    }
    else if(argument.size()==1){
        if(chdir(Home.c_str())!=0){
            perror("cant reach home directory");
        }
    }
    else{
        argument.pop();
        string flag=argument.front();
        if(flag==".."){
            if(chdir("..")!=0){
                perror("chdir not executed");
            }
        }
        else if(flag=="~"){
            const char* home = getenv("HOME");
            if(home==nullptr){
                perror("Home is not fetched");
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
}

void handle_pinfo(queue<string>argument){
    argument.pop();
    int temp;
    if(argument.size()>=2){
        cout<<"Invalid arguments"<<endl;
        return;
    }
    int pid;
    if(!argument.empty()){
        pid=stoi(argument.front());
        argument.pop();
    }
    else{
        pid=getpid();
    }
    int x[4];
    struct kinfo_proc p_info;
    unsigned long pinfosize=sizeof(p_info);
    x[0] = CTL_KERN;
    x[1] = KERN_PROC;
    x[2] = KERN_PROC_PID;
    x[3] = pid;
    temp=sysctl(x, 4, &p_info, &pinfosize, NULL, 0);
    if(temp!=0){
        cout<<"proces status not fetched"<<endl;
        return;
    }
    char status='?';
    if(p_info.kp_proc.p_stat==SRUN){
        status='R';
    }
    else if(p_info.kp_proc.p_stat==SSLEEP){
        status='S';
    }
    else if(p_info.kp_proc.p_stat==SZOMB){
        status='Z';
    }
    else if(p_info.kp_proc.p_stat==SSTOP){
        status='T';
    }
    int pgid=getpgid(pid);
    if(pgid==-1){
        perror("getpgid not executed");
        return;
    }
    if(pid==getpid()){
        int fg_pid=tcgetpgrp(STDIN_FILENO);
        if(pgid==fg_pid){
            cout<<"Process Status -- "<<status<<"+"<<endl;
        }
    }
    else{
        cout<<"Process Status -- "<<status<<endl;
    }
    struct proc_taskinfo tinfo;
    temp=proc_pidinfo(pid,PROC_PIDTASKINFO,0,&tinfo,sizeof(tinfo));
    if(temp<=0){
        cout<<"cant get process info"<<endl;
        return;
    }
    char path[PROC_PIDPATHINFO_MAXSIZE];
    temp=proc_pidpath(pid,path,sizeof(path));
    string exe_path=string(path);
    if(temp<=0){
        cout<<"Executable path cant fetched."<<endl;
        return;
    }
    if(exe_path.size()>=Home.size()){
        exe_path.replace(0,Home.size(),1,'~');
    }    
    unsigned long long virtual_mem=(unsigned long long)tinfo.pti_resident_size;
    cout<<"Memory -- "<<virtual_mem<<endl;
    cout<<"Executable Path -- "<<exe_path<<endl;
}

void execute_command(deque<trackbg>pipe_argument){
    if(pipe_argument.size()>=2){
        execute_pipe(pipe_argument);
        return;
    }
    while (!pipe_argument.empty()){
        trackbg current=pipe_argument.front(); 
        pipe_argument.pop_front();

        queue<string>argument= current.arg;
        bool background=current.bg;
        if(background==true && argument.empty()){
            cout<<"Invalid argument"<<endl;
            return;
        }
        if(argument.empty()) continue;
        if(!current.input.empty()||!current.output.empty()){
            io_redirection(argument,current.input,current.output,current.append);
            return;
        }
        if(background==true){
            handle_background(argument);
        }
        else if(background!=true && custom(argument.front())){
            handle_foreground(argument);
        }
        else{
            execute_inbuilt(argument);
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
        if(hist.size()<20){
            hist.push_back(string(token));
        }
        else{
            hist.erase(hist.begin());
            hist.push_back(string(token));
        }
        token=strtok(nullptr,delimiter);
    }
    delete[]input_copy;
    // Pipe seperated
    const char* delimiter2="|";
    while(!command_split.empty()){
        string s=command_split.front();
        command_split.pop();
        queue<string>pipe_split;
        char* command=new char[s.length()+1];
        strcpy(command,s.c_str());
        char* token2=strtok(command,delimiter2);
        while(token2!=nullptr){
            pipe_split.push(string(token2));
            token2=strtok(nullptr,delimiter2);
        }
        delete[]command;
        deque<trackbg>pipe_argument;
        // Space seperated
        const char* delimiter3=" \t";
        while(!pipe_split.empty()){
            string s2=pipe_split.front();
            pipe_split.pop();
            char* command1=new char[s2.length()+1];
            strcpy(command1,s2.c_str());
            char* token3=strtok(command1,delimiter3);
            trackbg tb;
            while(token3!=nullptr){
                string temp=string(token3);
                if(temp=="<"){
                    token3=strtok(nullptr,delimiter3);
                    if(token3!=nullptr)tb.input=string(token3);
                }
                else if(temp==">"){
                    token3=strtok(nullptr,delimiter3);
                    if(token3!=nullptr){
                        tb.output=string(token3);
                        tb.append=false;
                    }
                }
                else if(temp==">>"){
                    token3=strtok(nullptr,delimiter3);
                    if(token3!=nullptr){
                        tb.output=string(token3);
                        tb.append=true;
                    }
                }
                else if(temp=="&"){
                    tb.bg=true;
                }
                else{
                    tb.arg.push(temp);
                }
                token3=strtok(nullptr,delimiter3);
            }
            if(!tb.arg.empty()){
                pipe_argument.push_back(tb);
            }
            delete[] command1;
        }
        execute_command(pipe_argument);
    }    
}

void get_input(){
    rl_bind_key('\t', rl_complete);
    char *input=readline(system_name.c_str());
    if(!input) exit(0);
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
