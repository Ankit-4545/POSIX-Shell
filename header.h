#ifndef function_h
#define function_h
#include<queue>
using namespace std;
extern queue<string>command_split;
extern string Home;

void handle_ls(queue<string>space_split);
void handle_search(queue<string>space_split);
#endif
