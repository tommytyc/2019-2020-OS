/*
Student No.: 0616222
Student Name: 黃立銘
Email: imsohandsome123@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;

// cd "/mnt/e/Visual Studio Project/LiMing/LiMing"
// g++ -o os_hw5.out os_hw5.cpp
// ./os_hw5.out whois.txt
// ./os_hw5.out zipf.txt
// ./os_hw5.out ./testcase/zipf.txt
int main(int argc, char* argv[]) {
	fstream fin;
	struct timeval Tstart, Tend;
	int input, frame;//input value from file
	//LFU
	cout << "LFU policy" << endl;
	cout << "Frame\tHit\tMiss\tPage fault ratio" << endl;
	gettimeofday(&Tstart, 0); 
	unordered_map<int, int> LFUhash;
	unordered_map<int, list<int> > freq;
	unordered_map<int, list<int>::iterator> iter;
	for (frame = 64; frame <= 512; frame *= 2) {
		fin.open(argv[argc - 1], ios::in);
		int hit = 0, miss = 0, minfreq = 0;
		double  ratio = 0;
		while (fin >> input) {
			if (LFUhash.count(input) != 0) {//input is in LFUlist
				hit++;
				freq[LFUhash[input]].erase(iter[input]);
				LFUhash[input]++;
				freq[LFUhash[input]].push_back(input);
				iter[input] = --freq[LFUhash[input]].end();
				if (freq[minfreq].size() == 0) ++minfreq;

			}
			else if (LFUhash.size() < frame) {// not found and still empty space in frame
				miss++;
				LFUhash[input] = 1;
				freq[1].push_back(input);
				iter[input] = --freq[1].end();
				minfreq = 1;
			}
			else { // not found and frame is full
				miss++;
				LFUhash.erase(freq[minfreq].front());
				iter.erase(freq[minfreq].front());
				freq[minfreq].pop_front();
				LFUhash[input] = 1;
				freq[1].push_back(input);
				iter[input] = --freq[1].end();
				minfreq = 1;
			}
		}
		ratio = (double) miss / (hit + miss);
		cout << frame << "\t" << hit << "\t" << miss << "\t" << fixed << setprecision(10) << ratio << endl;
		LFUhash.clear();
		freq.clear();
		iter.clear();
		fin.close();
	}
	gettimeofday(&Tend, 0);
	double Tdifference = (Tend.tv_sec - Tstart.tv_sec) + (Tend.tv_usec - Tstart.tv_usec) / 1000000.0;
	cout << "Total elapsed time " << Tdifference << " secs\n\n";

	//LRU
	cout << "LRU policy" << endl;
	cout << "Frame\tHit\tMiss\tPage fault ratio" << endl;
	gettimeofday(&Tstart, 0); 
	for (frame = 64; frame <= 512; frame *= 2) {
		fin.open(argv[argc - 1], ios::in);
		list<int> LRUlist;
		unordered_map<int, list<int>::iterator> LRUhash;
		unordered_map<int, list<int>::iterator>::iterator iter;
		int hit = 0, miss = 0, len = 0;
		double  ratio = 0;
		while (fin >> input) {
			iter = LRUhash.find(input);
			if (iter != LRUhash.end()) {//input is in LRUlist
				hit++;
				LRUlist.erase(iter->second);
				LRUlist.push_front(input);
				LRUhash[input] = LRUlist.begin();
			}
			else if (len < frame) {// not found and still empty space in frame
				miss++;
				LRUlist.push_front(input);
				LRUhash[input] = LRUlist.begin();
				len++;
			}
			else { // not found and frame is full
				miss++;
				LRUhash.erase(LRUlist.back());
				LRUlist.pop_back();
				LRUlist.push_front(input);
				LRUhash[input] = LRUlist.begin();
			}
		}
		ratio = (double) miss / (hit + miss);
		cout << frame << "\t" << hit << "\t" << miss << "\t" << fixed << setprecision(10) << ratio << endl;
		LRUhash.clear();
		LRUlist.clear();
		fin.close();
	}
	gettimeofday(&Tend, 0);
	Tdifference = (Tend.tv_sec - Tstart.tv_sec) + (Tend.tv_usec - Tstart.tv_usec) / 1000000.0;
	cout << "Total elapsed time " << Tdifference << " secs\n\n";
}