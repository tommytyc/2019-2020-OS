/*
Student No.: 0616078
Student Name: 唐宇謙
Email: tommytyc.cs06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
using namespace std;

int main(){
	pid_t pid;
	pid = fork();
	//fork another process
	if(pid < 0){//for error situation
		cerr<<"Fork Failed!\n";
		exit(-1);
	}
	else if(pid == 0){//in child process
		while(1){
			pid_t tmppid;
			tmppid = fork();
			if(tmppid < 0){
				cerr<<"Child Process Fork Failed!\n";
			}
			else if(tmppid == 0){
				char* cstr1 = new char[256];
				cout<<">";
				cin.getline(cstr1, 256);
				execl("/bin/sh", "sh", "-c", cstr1, NULL);
				cout<<"successfully executed!\n";
			}
			else{
				wait(NULL);
			}
		}
		exit(0);
	}
	else{//in parent process
		wait(NULL);
		exit(0);
	}
	return 0;
}
