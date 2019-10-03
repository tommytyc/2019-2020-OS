/*
Student No.: 0616078
Student Name: 唐宇謙
Email: tommytyc.cs06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>
using namespace std;

int main(){
	pid_t pid;
	int exv = 0;// the return value of execvp
	bool cnt = false;
	char* p;
	pid = fork();
	//fork another process
	if(pid < 0){//for error situation
		cerr<<"Fork Failed!\n";
		exit(-1);
	}
	else if(pid == 0){//in child process
		while(1){
			char* cstr1 = new char[256];
			cout<<">";
			cin.getline(cstr1, 256);
			int status;
			status = system(cstr1);
			if(status == -1){
				cerr<<"Error.\n";
			}
		}
		exit(0);		
	}
	else{//in parent process
		wait(NULL);
	}
	return 0;
}
