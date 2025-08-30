#include<iostream>
#include<unistd.h>
#include<dirent.h>
#include<iomanip>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include"header.h"
using namespace std;

void print_all_entries(string flag){
    DIR* dir_stream=opendir(flag.c_str());
    if(dir_stream==nullptr){
        perror("Opendir not executed");
        return;
    }
    struct dirent* dir;
    while((dir=readdir(dir_stream))!=nullptr){
        cout<<dir->d_name<<endl;
    }
}

void print_entries(string flag){
    DIR* dir_stream=opendir(flag.c_str());
    if(dir_stream==nullptr){
        perror("Opendir not executed");
        return;
    }
    struct dirent* dir;
    while((dir=readdir(dir_stream))!=nullptr){
        if(dir->d_name[0]=='.'){
            continue;
        }
        cout<<dir->d_name<<endl;
    }
}

void print_long_format(string present){
    struct stat buf;
    string info="__________";
    DIR* file=opendir(present.c_str());
    if(file==nullptr){
        perror("opendir not executed");
        return;
    }
    struct dirent* entry;
    while((entry=readdir(file))!=nullptr){
        if(entry->d_name[0]=='.'){
            continue;
        }
        if(stat(present.c_str(),&buf)!=0){
            perror("stat not executed");
        }
        info[0]=S_ISDIR(buf.st_mode)?'d':'_';
        info[1]=(buf.st_mode & S_IRUSR)?'r':'_';
        info[2]=(buf.st_mode & S_IWUSR)?'w':'_';
        info[3]=(buf.st_mode & S_IXUSR)?'x':'_';
        info[4]=(buf.st_mode & S_IRGRP)?'r':'_';
        info[5]=(buf.st_mode & S_IWGRP)?'w':'_';
        info[6]=(buf.st_mode & S_IXGRP)?'x':'_';
        info[7]=(buf.st_mode & S_IROTH)?'r':'_';
        info[8]=(buf.st_mode & S_IWOTH)?'w':'_';
        info[9]=(buf.st_mode & S_IXOTH)?'x':'_';
        cout<<setw(10)<<info<<" ";
        cout<<setw(3)<<buf.st_nlink<<" "; 
        int u_id=buf.st_uid;
        string u_name=getpwuid(u_id)->pw_name;
        cout<<setw(15)<<u_name<<" ";
        int gr_id=buf.st_gid;
        string gr_name=getgrgid(gr_id)->gr_name;
        cout<<setw(10)<<gr_name<<" ";
        cout<<setw(10)<<buf.st_size<<" ";
        char time[20];
        strftime(time, sizeof(time), "%d %b %H:%M", localtime(&buf.st_mtime));
        cout<<setw(10)<<time<<" ";
        cout<<setw(10)<<entry->d_name<<" ";
        cout<<endl;
    }    
}

void print_all_long_format(string present){
    struct stat buf;
    string info="__________";
    DIR* file=opendir(present.c_str());
    if(file==nullptr){
        perror("opendir not executed");
        return;
    }
    struct dirent* entry;
    while((entry=readdir(file))!=nullptr){
        if(stat(present.c_str(),&buf)!=0){
            perror("stat not executed");
        }
        info[0]=S_ISDIR(buf.st_mode)?'d':'_';
        info[1]=(buf.st_mode & S_IRUSR)?'r':'_';
        info[2]=(buf.st_mode & S_IWUSR)?'w':'_';
        info[3]=(buf.st_mode & S_IXUSR)?'x':'_';
        info[4]=(buf.st_mode & S_IRGRP)?'r':'_';
        info[5]=(buf.st_mode & S_IWGRP)?'w':'_';
        info[6]=(buf.st_mode & S_IXGRP)?'x':'_';
        info[7]=(buf.st_mode & S_IROTH)?'r':'_';
        info[8]=(buf.st_mode & S_IWOTH)?'w':'_';
        info[9]=(buf.st_mode & S_IXOTH)?'x':'_';
        cout<<setw(10)<<info<<" ";
        cout<<setw(3)<<buf.st_nlink<<" "; 
        int u_id=buf.st_uid;
        string u_name=getpwuid(u_id)->pw_name;
        cout<<setw(15)<<u_name<<" ";
        int gr_id=buf.st_gid;
        string gr_name=getgrgid(gr_id)->gr_name;
        cout<<setw(10)<<gr_name<<" ";
        cout<<setw(10)<<buf.st_size<<" ";
        char time[20];
        strftime(time, sizeof(time), "%d %b %H:%M", localtime(&buf.st_mtime));
        cout<<setw(10)<<time<<" ";
        cout<<setw(10)<<entry->d_name<<" ";
        cout<<endl;
    }    
}

void handle_ls(){
    if(space_split.size()==1){
        print_entries(".");
        return;
    }
    else{
        char cwd[256];
        string present=string(getcwd(cwd,256));
        string path=".";
        space_split.pop();
        bool show_all=false;
        bool show_long=false;
        while(!space_split.empty()){
            string flag=space_split.front();
            if(flag==".."||flag=="."){
                path=flag;
            }
            else if(flag=="~"){
                path=present;
            }
            else if (flag=="-a"){
                show_all=true;    
            }
            else if(flag=="-l"){
                show_long=true;
            }
            else if(flag=="-al"||flag=="-la"){
                show_all=true;
                show_long=true;
            }
            else{
                path=flag;
            }
            space_split.pop();
        }
        if(show_all && show_long){
            print_all_long_format(path);
        }
        else if(show_all){
            print_all_entries(path);
        }
        else if(show_long){
            print_long_format(path);
        }
        else{
            print_entries(path);
        }
    }
}