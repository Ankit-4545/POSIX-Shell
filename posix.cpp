#include<iostream>
#include<termios.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<cstring>
#include<stdlib.h>
using namespace std;

struct termios original;


void enableraw() {
    tcgetattr(0, &original);
    struct termios raw = original;
    raw.c_lflag &= ~(ICANON|ECHO);                     
    tcsetattr(0, TCSAFLUSH, &raw);     
}

void disableraw() {
    tcsetattr(0, TCSAFLUSH, &original); 
}

void handle_echo(char input[]){
    const char *delimiters=" \t";
    char *token=strtok(input,delimiters);
    token=strtok(nullptr,delimiters);
    while (token!=nullptr)
    {
        cout<<token<<" ";
        token=strtok(nullptr,delimiters);
    }   
}

void handle_pwd(){
    char current_directory[256];
    if(getcwd(current_directory,256)==nullptr){
        cout<<"current directory not fetched"<<endl;
    }
    cout<<current_directory<<endl;
    cout<<flush;
}

string format_cwd(){
    char cwd[256];
    char *ptr=getcwd(cwd,256);
    string s="~";
    if(ptr==nullptr){
        cout<<"cwd not executed"<<endl;
        return "";
    }
    char ch=*(ptr+strlen(cwd));
    if(!ch){
        return s+ch;
    }
    return "";
}

void print_prompt()
{
    char *u_name = getenv("USER");
    if (u_name == nullptr)
    {
        cout << "User name not found" << endl;
    }
    string str = format_cwd();
    char host_name[256];
    int x = gethostname(host_name, 256);
    if (x == -1)
    {
        cout << "User name can't be fetched" << endl;
        return;
    }
    string k=(string)u_name+"@"+host_name+":"+str;
    const char * p = k.c_str();
    write(1,p,strlen(p));
}

void get_input() {
    string input = "";
    while (true) {
        char ch = 0;
        if (read(0, &ch, 1) != 1) {
            perror("read not executed");
            break;
        }
        if (ch == 127 || ch == 8) {
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
                cout.flush();
            }
            continue;
        }
        if (ch == '\n') {
            cout << endl;
            const char *delimiters = ";\t ";
            char *inp = new char[input.size() + 1];
            strcpy(inp, input.c_str());
            char *token = strtok(inp, delimiters);
            if (!token) {
                delete[] inp;
                break;
            }
            if (strcmp(token, "exit") == 0) {
                delete[] inp;
                exit(0);
            }
            if (strcmp(token, "echo") == 0) {
                char *inp_copy = new char[input.size() + 1];
                strcpy(inp_copy, input.c_str());
                handle_echo(inp_copy);
                delete[] inp_copy;
            }
            if (strcmp(token, "pwd") == 0) {
                handle_pwd();
            }
            delete[] inp;
            break;
        }
        input += ch;
        cout << ch << flush;
    }
}

int main(){
    enableraw(); 
    while(true){
        print_prompt();
        get_input();
        
    }
    atexit(disableraw);     
    return 0;
}
