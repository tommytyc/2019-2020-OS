/*
Student No.: <your student id>
Student Name: <your name>
Email: <your email>
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/

#include <bits/stdc++.h>
#include <iomanip>
#include <fstream>
#include <sys/time.h>

using namespace std;

class pqpair {
public:
	pqpair(){};
	pqpair(int p, int c, int t){
		page = p;
		cnt = c;
		time = t;
	};
	int page, cnt, time;
};

int main(int argc, char *argv[]){
	fstream fi;
	struct timeval start, end;
	if(argc < 2){
		cout<<"usage: ./0616078.out [input_file_name]"<<endl;
		return 0;
	}
	char* input = argv[1];

	//begin LFU
	cout<<"LFU policy:\n";
	cout<<"Frame\tHit\t\tMiss\t\tPage fault ratio\n";
	int hit = 0, miss = 0, tmp = 0, timestamp = 0, len = 0;
	double pfratio = 0;
	gettimeofday(&start, 0);
	for(int fnumber = 64; fnumber <= 512; fnumber *= 2){
		hit = 0; miss = 0; tmp = 0; timestamp = 0; pfratio = 0; len = 0;
		fi.open(input, ios::in);
		map<int, pqpair> hashall;
		map<int, pqpair>::iterator itall;
		list<pqpair> pq;
		list<pqpair>::iterator itl1, itl2;
		map<int, list<pqpair>::iterator> hashpq;
		map<int, list<pqpair>::iterator>::iterator itm;
		while(fi>>tmp){
			timestamp++;
			itall = hashall.find(tmp);
			if(itall == hashall.end()){
				hashall[tmp] = pqpair(tmp, 1, timestamp);
			}
			else{
				itall->second.cnt++;
				itall->second.time = timestamp;
			}
			itm = hashpq.find(tmp);
			if(itm != hashpq.end()){			//in the list
				hit++;
				hashpq[tmp]->cnt++;
				hashpq[tmp]->time = timestamp;
				for(itl1 = hashpq[tmp]; itl1->page != pq.back().page; itl1++){
					itl1++; itl2 = itl1; itl1--;
					if((*itl1).cnt > (*itl2).cnt || ((*itl1).cnt == (*itl2).cnt && (*itl1).time > (*itl2).time)){
						hashpq[(*itl2).page] = itl1;
						hashpq[(*itl1).page] = itl2;
						(*itl1).page^=(*itl2).page^=(*itl1).page^=(*itl2).page;
						(*itl1).cnt^=(*itl2).cnt^=(*itl1).cnt^=(*itl2).cnt;
						(*itl1).time^=(*itl2).time^=(*itl1).time^=(*itl2).time;
					}
					else{
						hashpq[tmp] = itl1;
						break;
					}
				}
			}
			else if(len < fnumber){				//not in the list, but no need to replace page
				miss++;
				pqpair *tmppq = new pqpair(tmp, 1, timestamp);
				pq.push_front(*tmppq);
				len++;
				hashpq[tmp] = pq.begin();
				delete tmppq;
				for(itl1 = hashpq[tmp]; itl1->page != pq.back().page; itl1++){
					itl1++; itl2 = itl1; itl1--;				
					if((*itl1).cnt > (*itl2).cnt || ((*itl1).cnt == (*itl2).cnt && (*itl1).time > (*itl2).time)){
						hashpq[(*itl2).page] = itl1;
						hashpq[(*itl1).page] = itl2;
						(*itl1).page^=(*itl2).page^=(*itl1).page^=(*itl2).page;
						(*itl1).cnt^=(*itl2).cnt^=(*itl1).cnt^=(*itl2).cnt;
						(*itl1).time^=(*itl2).time^=(*itl1).time^=(*itl2).time;
					}
					else{
						hashpq[tmp] = itl1;
						break;
					}
					
				}
			}
			else{								//not in the list, and need to replace page
				miss++;
				if(hashall[tmp].cnt > pq.front().cnt || (hashall[tmp].cnt == pq.front().cnt && hashall[tmp].time > pq.front().time)){
					hashall[pq.front().page].cnt = 0;
					hashall[pq.front().page].time = 0;
					hashpq.erase(pq.front().page);
					pq.pop_front();
					pq.push_front(hashall[tmp]);
					hashpq[tmp] = pq.begin();
					for(itl1 = hashpq[tmp]; itl1->page != pq.back().page; itl1++){
						itl1++; itl2 = itl1; itl1--;
						if((*itl1).cnt > (*itl2).cnt || ((*itl1).cnt == (*itl2).cnt && (*itl1).time > (*itl2).time)){
							hashpq[(*itl2).page] = itl1;
							hashpq[(*itl1).page] = itl2;
							(*itl1).page^=(*itl2).page^=(*itl1).page^=(*itl2).page;
							(*itl1).cnt^=(*itl2).cnt^=(*itl1).cnt^=(*itl2).cnt;
							(*itl1).time^=(*itl2).time^=(*itl1).time^=(*itl2).time;
						}
						else{
							hashpq[tmp] = itl1;
							break;
						}
					}

				}

			}
		}

		pfratio = (double)miss / (miss + hit);
		cout<<fnumber<<"\t"<<hit<<"\t\t"<<miss<<"\t\t";
		cout<<fixed<<setprecision(10)<<pfratio<<endl;
		fi.close();
	}
	gettimeofday(&end, 0);
	int sec = end.tv_sec - start.tv_sec;
	int usec = end.tv_usec - start.tv_usec;
	cout<<"Total elapsed time "<<fixed<<setprecision(4)<<(sec * 1000 + (usec/1000.0))/1000<<" secs\n\n";
	cout.unsetf( ios::fixed );

	//begin LRU
	cout<<"LRU policy:\n";
	cout<<"Frame\tHit\t\tMiss\t\tPage fault ratio\n";
	gettimeofday(&start, 0);
	for(int fnumber = 64; fnumber <= 512; fnumber *= 2){
		fi.open(input, ios::in);
		hit = 0; miss = 0; tmp = 0;
		double pfratio;
		map<int, list<int>::iterator> hash;
		list<int> llist;
		map<int, list<int>::iterator>::iterator it;

		while(fi>>tmp){
			it = hash.find(tmp);
			if(it != hash.end()){					//in the list
				hit++;
				llist.erase(it->second);
				llist.push_front(tmp);
				hash[tmp] = llist.begin();
			}
			else if(llist.size() < fnumber){		//not in the list, but no need to replace page
				miss++;
				llist.push_front(tmp);
				hash[tmp] = llist.begin();
			}
			else{									//not in the list, and need to replace page
				miss++;
				hash.erase(llist.back());
				llist.pop_back();
				llist.push_front(tmp);
				hash[tmp] = llist.begin();
			}
		}
		pfratio = (double)miss / (miss + hit);
		cout<<fnumber<<"\t"<<hit<<"\t\t"<<miss<<"\t\t";
		cout<<fixed<<setprecision(10)<<pfratio<<endl;
		fi.close();
	}
	gettimeofday(&end, 0);
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	cout<<"Total elapsed time "<<fixed<<setprecision(4)<<(sec * 1000 + (usec/1000.0))/1000<<" secs\n\n";
	cout.unsetf( ios::fixed );

}