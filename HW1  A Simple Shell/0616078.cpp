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
	char cstr[] = "";
	char tmp[256];
	char* p;
	pid = fork();
	//fork another process
	if(pid < 0){//for error situation
		cerr<<"Fork Failed!\n";
		exit(-1);
	}
	else if(pid == 0){//in child process
		while(1){
			cstr = "";
			cin.getline(cstr, 256);
			p = strtok(cstr, " ");
			while(p != NULL){
				if(cnt){
					execlp(cstr, cstr, p, NULL);
					break;
				}
				else{
					p = strtok(NULL, "");
					cnt = true;
				}
				
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
