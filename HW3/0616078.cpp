/*
Student No.: 0616078
Student Name: 唐宇謙
Email: tommytyc.cs06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 2147483647
using namespace std;

vector<int>::iterator it;

typedef struct{
	int f;
	int e;
}pmt;

pmt par[16];
sem_t ending;
sem_t sem[16];

void* bubble(void* idp);
void sbubble(vector<int> &arr, int first, int end);
void merge(vector<int> &arr, int f1, int e1, int f2, int e2);
void STsort(vector<int> &arr, int first, int end, int times);
void* MTsort(void* idp);

void* bubble(void* idp){
	int id = *(int*) idp;
	sem_wait(&sem[id]);
	for(int i = par[id].f; i <= par[id].e; i++){
		for(int j = par[id].f; j < par[id].e; j++){
			if(*(it + j) > *(it + j + 1)){
				(*(it + j))^=(*(it + j + 1))^=(*(it + j))^=(*(it + j + 1));			
			}
		}
	}
	sem_post(&sem[id + 15]);
}

void sbubble(vector<int> &arr, int first, int end){//single thread bubble sort
	for(int i = first; i <= end; i++){
		for(int j = first; j < end; j++){
			if(arr[j] > arr[j + 1]){
				arr[j]^=arr[j + 1]^=arr[j]^=arr[j + 1];
			}
		}
	}
	return;
}

void merge(vector<int> &arr, int f1, int e1, int f2, int e2){
	vector<int> left;
	vector<int> right;
	left.assign(arr.begin() + f1, arr.begin() + e1 + 1);
	right.assign(arr.begin() + f2, arr.begin() + e2 + 1);
	left.push_back(MAX);
	right.push_back(MAX);
	int lptr = 0, rptr = 0, arrptr = f1;
	while(lptr < left.size() - 1 || rptr < right.size() - 1){
		if(left.at(lptr) < right.at(rptr)){
			arr.at(arrptr) = left.at(lptr);
			lptr++;
		}
		else{
			arr.at(arrptr) = right.at(rptr);
			rptr++;
		}
		arrptr++; 
	}
}

void STsort(vector<int> &arr, int first, int end, int times){//single thread sorting
	if(times <= 3){
		STsort(arr, first, first + (end - first + 1)/2, ++times);
		STsort(arr, first + (end - first + 1)/2 + 1, end, ++times);
		merge(arr, first, first + (end - first + 1)/2, first + (end - first + 1)/2 + 1, end);
	}
	else
		sbubble(arr, first, end);
	return;
}

void* MTsort(void* idp){
	int id = *(int*) idp;
	sem_wait(&sem[id]);
	sem_post(&sem[id*2]);
	sem_post(&sem[id*2 + 1]);
	sem_wait(&sem[id*2 + 15]);
	sem_wait(&sem[(id*2 + 1) + 15]);
	vector<int> left;
	vector<int> right;
	left.assign(it + par[id].f, it + par[id*2].e + 1);
	right.assign(it + par[id*2 + 1].f, it + par[id].e + 1);
	left.push_back(MAX);
	right.push_back(MAX);
	int lptr = 0, rptr = 0, arrptr = par[id].f;
	while(lptr < left.size() - 1 || rptr < right.size() - 1){
		if(left.at(lptr) < right.at(rptr)){
			*(it + arrptr) = left.at(lptr);
			lptr++;
		}
		else{
			*(it + arrptr) = right.at(rptr);
			rptr++;
		}
		arrptr++; 
	}
	if(id != 1){
		sem_post(&sem[id + 15]);
	}
	else{
		sem_post(&ending);
	}
}

int main(){
	string fname;
	cout<<"Enter input file name:";
	cin>>fname;

	fstream fi, fo1, fo2;
	fi.open(fname, ios::in);
	fo1.open("output1.txt", ios::out);
	fo2.open("output2.txt", ios::out);
	int n = 0;
	struct timeval start, end;
	fi>>n;
	vector<int> num(n);
	for(int i = 0; i < n; i++){
		fi>>num[i];
	}
	vector<int> stnum;
	stnum.assign(num.begin(), num.end());

	//ST
	gettimeofday(&start, 0);
	STsort(stnum, 0, n - 1, 1);
	gettimeofday(&end, 0);
	int sec = end.tv_sec - start.tv_sec;
	int usec = end.tv_usec - start.tv_usec;
	cout<<"ST sorting used "<<(sec * 1000 + (usec/1000.0))/1000<<" secs\n";
	for(int i = 0; i <= n - 1; i++){
		fo2<<stnum.at(i)<<" ";
	}
	fo2.close();
	

	//MT
	pthread_t thread[31];
	par[1].f = 0; par[1].e = (n - 1);
	for(int i = 1; i <= 15; i++){
		sem_init(&sem[i], 0, 0);
	}
	sem_init(&ending, 0, 0);
	for(int i = 2; i < 16; i++){
		if(i % 2 == 0){
			par[i].f = par[i/2].f; par[i].e = (par[i/2].f + (par[i/2].e - par[i/2].f + 1)/2);
			par[i + 1].f = par[i].e + 1; par[i + 1].e = par[i/2].e;
		}
		else{
			continue;
		}
	}
	it = num.begin();
	int tmp[16];
	for(int i = 0; i < 16; i++){
		tmp[i] = i;
	}
	gettimeofday(&start, 0);
	for(int i = 1; i <= 15; i++){
		int ret;
		if(i < 8){
			ret = pthread_create(&thread[i], NULL, MTsort, &tmp[i]);
		}
		else{
			ret = pthread_create(&thread[i], NULL, bubble, &tmp[i]);
		}
		if(ret != 0){
			cout<<"create "<<i<<" thread failed.\n";
		}
	}
	sem_post(&sem[1]);
	sem_wait(&ending);
	gettimeofday(&end, 0);

	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	cout<<"MT sorting used "<<(sec * 1000 + (usec/1000.0))/1000<<" secs\n";
	for(; it != num.end(); it++){
		fo1<<*it<<" ";
	}

	fi.close();
	fo1.close();
	return 0;
}