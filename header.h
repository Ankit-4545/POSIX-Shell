#ifndef function_h
#define function_h
#include<vector>
#include<queue>
#include<deque>
#include<dirent.h>
#include<iomanip>
#include<sys/utsname.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/stat.h>
#include<sys/sysctl.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<fcntl.h>
#include<libproc.h>
#include<unistd.h>
#include<queue>
using namespace std;
extern vector<string>hist;
extern queue<string>command_split;
extern string Home;

struct trackbg{
    queue<string>arg;
    bool bg=false;
    string input="";
    string output="";
    bool append=false; 
};

void io_redirection(queue<string>argument,string input,string output,bool append);
void execute_inbuilt(queue<string>argument);
void handle_ls(queue<string>argument);
void handle_search(queue<string>argument);
void handle_echo(queue<string>argument);
void handle_pinfo(queue<string>argument);
void handle_ls(queue<string>argument);
void handle_pwd(queue<string>argument);
void handle_cd(queue<string>argument);
void handle_foreground(queue<string>argument);
void handle_background(queue<string>argument);
void execute_pipe(deque<trackbg>pipe_argument);
void show_history(queue<string>argument);
int custom(string command);
#endif
