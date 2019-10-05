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
			pid_t tmppid;
			bool andflag = false;
			tmppid = fork();
			if(tmppid < 0){//fork error
				cerr<<"Child Process Fork Failed!\n";
			}
			else if(tmppid == 0){//in child process
				char* cstr1 = new char[256];
				vector<string> vstr;
				cout<<">";
				cin.getline(cstr1, 256);
				p = strtok(cstr1, " ");
				while(p != NULL){
					vstr.push_back(p);
					p = strtok(NULL, " ");
				}
				delete [] cstr1;

				//dynamic memory allocation
				char** cstr2 = new char*[vstr.size()+1];
				for(int i = 0; i <= vstr.size(); i++){
					cstr2[i] = new char[256];
				}

				//copy the string
				for(int i = 0; i < vstr.size(); i++){
					if(vstr[i] == "&"){
						andflag = true;
					}
					strcpy(cstr2[i], vstr[i].c_str());
				}
				cstr2[vstr.size()] = 0;
				//check whether the command has "&"
				if(andflag){
					for(int i = 0; i <= vstr.size(); i++){
						cstr2[i] = 0;
					}
				}
				exv = execvp(cstr2[0], cstr2);
			}
			else{//in parent process
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