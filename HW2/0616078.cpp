/*
Student No.: 0616078
Student Name: 唐宇謙
Email: tommytyc.cs06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <iostream>
#define L 16
using namespace std;

int main(){
	int dim = 0;
	cout<<"Input the matrix dimension: ";
	cin>>dim;

	char* shmaddr;
	int shmida = 0, shmidb = 0, shmidc = 0, status = 0;
	struct timeval start, end;
	pid_t pid, wpid;
	unsigned int* A;
	unsigned int* B;
	unsigned int* C;

	//get the shared memory address
	shmida = shmget(IPC_PRIVATE, sizeof(unsigned int[dim][dim]), IPC_CREAT|0600);
	shmidb = shmget(IPC_PRIVATE, sizeof(unsigned int[dim][dim]), IPC_CREAT|0600);
	shmidc = shmget(IPC_PRIVATE, sizeof(unsigned int[dim][dim]), IPC_CREAT|0600);

	//attach it!
	A = (unsigned int*)shmat(shmida, NULL, 0);
	B = (unsigned int*)shmat(shmidb, NULL, 0);
	C = (unsigned int*)shmat(shmidc, NULL, 0);

	unsigned cnter = 0;
	for(unsigned int i = 0; i < dim; i++){
		for(unsigned int j = 0; j < dim; j++){
			A[i * dim + j] = cnter;
			B[i * dim + j] = cnter;
			cnter++;
		}
	}

	signal(SIGCHLD, SIG_IGN);

	for(unsigned int T = 1; T <= L; T++){//T is the number of partition

		for(unsigned int i = 0; i < dim; i++){//memset C = 0
			for(unsigned int j = 0; j < dim; j++){
				C[i * dim + j] = 0;
			}
		}
		unsigned int P = 0;
		for(P = 1; P <= T; P++){//P is the partition with which the process need to deal
			pid = fork();
			if(pid == 0 || pid == -1){
				break;
			}
		}
		if(pid == 0){
			unsigned int tmp = 0;
			
			if(P != T){
				for(unsigned int i = (P-1)*(dim/T); i < P*(dim/T); i++){
					for(unsigned int j = 0; j < dim; j++){
						for(unsigned int k = 0; k < dim; k++){
							tmp += (A[i * dim + k] * B[j + k * dim]);
						}
						C[i * dim + j] = tmp;
						tmp = 0;
					}
				}
			}
			else{
				for(unsigned int i = (P-1)*(dim/T); i < dim; i++){
					for(unsigned int j = 0; j < dim; j++){
						for(unsigned int k = 0; k < dim; k++){
							tmp += (A[i * dim + k] * B[j + k * dim]);
						}
						C[i * dim + j] = tmp;
						tmp = 0;
					}
				}
			}
			exit(0);
		}
		else{
			gettimeofday(&start, 0);
			//wait for the termination of all child process
			while ((wpid = wait(&status)) > 0);
			gettimeofday(&end, 0);
			int sec = end.tv_sec - start.tv_sec;
			int usec = end.tv_usec - start.tv_usec;

			unsigned int tmp = 0;
			for(unsigned int i = 0; i < dim; i++){
				for(unsigned int j = 0; j < dim; j++){
					tmp += C[i * dim + j];
				}
			}
			cout<<"Multiplying matrices using "<<T<<" process";
			if(T != 1)cout<<"es";
			cout<<endl;
			cout<<"Elapsed time: "<<(sec * 1000 + (usec/1000.0))/1000<<"sec, Checksum: "<<tmp<<endl;
			
		}
	}
	
	//detach SharedMemory and delete	
	shmdt(A);
	shmdt(B);
	shmdt(C);

	shmctl(shmida, IPC_RMID, NULL);
	shmctl(shmidb, IPC_RMID, NULL);
	shmctl(shmidc, IPC_RMID, NULL);

	return 0;
}