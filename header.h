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
#include<libproc.h>
#include<unistd.h>
#include<queue>
using namespace std;
extern queue<string>command_split;
extern string Home;

struct trackbg{
    queue<string>arg;
    bool bg=false;
};

void handle_ls(queue<string>argument);
void handle_search(queue<string>argument);
void handle_echo(queue<string>argument);
void handle_ls(queue<string>argument);
void handle_pwd(queue<string>argument);
void handle_cd(queue<string>argument);
void handle_foreground(queue<string>argument);
void handle_background(queue<string>argument);
void execute_pipe(deque<trackbg>pipe_argument);
int custom(string command);
#endif
