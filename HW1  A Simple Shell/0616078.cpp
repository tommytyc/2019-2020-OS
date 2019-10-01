/*
Student No.: 0616078
Student Name: 唐宇謙
Email: .tommytyc.cs06@nctu.edu.tw
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
using namespace std;

int main(){
	pid_t pid;
	bool cnt = false;
	char* p;
	pid = fork();
	//fork another process
	if(pid < 0){//for error situation
		cerr<<"Fork Failed!\n";
		exit(-1);
	}
	else if(pid == 0){//in child process
			char* cstr = new char[256];
			cout<<">";
			cin.getline(cstr, 256);
			p = strtok(cstr, " ");
			while(1){
				if(cnt){
					execlp(cstr, cstr, p, NULL);
				}
				else{
					p = strtok(NULL, "");
					cnt = true;
				}
			}
	}
	else{//in parent process
		wait(NULL);
		// exit(0);
	}
	return 0;
}
