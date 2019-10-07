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

void Free(char** str, int size){
	for(int i = 0; i < size; i++){
		delete [] str[i];
	}
	delete [] str;
}

int main(){
	pid_t pid;
	int exv = 0, pipepos = 0, redpos = 0;// the return value of execvp
	bool andflag = false, pipeflag = false, redflag = false;
	char* p;
	vector<string> vstr;
	while(1){
		//initialization
		vstr.clear();
		andflag = false; pipeflag = false; redflag = false;
		pipepos = 0, redpos = 0;

		int pipefd[2];
		char* cstr1 = new char[256];
		cout<<">";
		cin.getline(cstr1, 256);
		p = strtok(cstr1, " ");
		while(p != NULL){
			vstr.push_back(p);
			p = strtok(NULL, " ");
		}
		if(vstr.back() == "&"){
			vstr.pop_back();
			andflag = true;
		}
		delete [] cstr1;
		int len = vstr.size();

		//dynamic memory allocation
		char** cstr2 = new char*[len + 1];
		for(int i = 0; i <= len; i++){
			cstr2[i] = new char[256];
		}

		//copy the string
		for(int i = 0; i < len; i++){
			if(vstr[i] == "|"){
				pipe(pipefd);
				pipeflag = true;
				pipepos = i;
			}
			else if(vstr[i] == ">"){
				redflag = true;
				redpos = i;
			}	
			strcpy(cstr2[i], vstr[i].c_str());
		}

		cstr2[vstr.size()] = 0;

		pid = fork();
		if(!andflag){
			if(pipeflag){//deal with pipe
				//initialization
				char **cpystr1 = new char*[pipepos];
				char **cpystr2 = new char*[len - pipepos -1];
				for(int i = 0; i < pipepos; i++){
					cpystr1[i] = new char[256];
				}
				for(int i = 0; i < pipepos; i++){
					strcpy(cpystr1[i], cstr2[i]);
				}
				for(int i = 0; i < (len - pipepos - 1); i++){
					cpystr2[i] = new char[256];
				}
				for(int i = 0; i < (len - pipepos - 1); i++){
					strcpy(cpystr2[i], cstr2[pipepos + i + 1]);
				}
				
				if(pid < 0){
					cerr<<"Fork Error!\n";
				}
				else if(pid == 0){//child process
					close(pipefd[0]);
					dup2(pipefd[1], STDOUT_FILENO);
					close(pipefd[1]);
					exv = execvp(cpystr1[0], cpystr1);
				}
				else{
					close(pipefd[1]);
					dup2(pipefd[0], 0);
					close(pipefd[0]);
					wait(NULL);

					// free the allocated memory
					Free(cstr2, len);
					Free(cpystr1, pipepos);

					pid_t pid2;
					if((pid2 = fork()) < 0){
						cerr<<"Fork Error!\n";
					}
					else if(pid2 == 0){
						execvp(cpystr2[0], cpystr2);
					}
					else{
						wait(NULL);
						Free(cpystr2, (len - pipepos - 1));
					}
				}
			}
			else{
				if(pid < 0){
					cerr<<"Fork Error!\n";
				}
				else if(pid == 0){//child process
					exv = execvp(cstr2[0], cstr2);
				}
				else{
					wait(NULL);
					for(int i = 0; i <= len; i++){
						delete [] cstr2[i];
					}
					delete [] cstr2;
				}
			}
		}

		else{
			if(pid == 0){//child process
				pid_t pid2;
				if((pid2 = fork()) < 0){
					cerr<<"Fork Error!\n";
				}
				else if(pid2 == 0){//grandchild process, supervised by init once the child process exit.
					exv = execvp(cstr2[0], cstr2);
				}
				else{
					exit(0);//make the grandchild process be supervised by init
				}
			}
			else{
				wait(NULL);
			}
		}
	}
	return 0;
}