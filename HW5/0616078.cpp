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
	pqpair(int p, int c, int t){
		page = p;
		cnt = c;
		time = t;
	};
	int page, cnt, time;
	bool operator>(pqpair p);
};

bool pqpair::operator>(pqpair p){
	if(cnt > p.cnt){
		return true;
	}
	else if(cnt == p.cnt && time > p.time){
		return true;
	}
	else{
		return false;
	}	
}

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
		hit = 0; miss = 0; tmp = 0; timestamp = 0; pfratio = 0;
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
				pqpair* tmppq = new pqpair(tmp, 1, timestamp);
				hashall[tmp] = *tmppq;
				delete tmppq;
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
				for(itl1 = pq.begin(); itl1 != pq.end(); itl1++){
					itl2 = (itl1++);
					itl1--;
					if(*itl1 > *itl2){
						pqpair pqtmp = *itl2;
						itl1 = itl2;
						*itl2 = pqtmp;
					}
					if(itl1->page == tmp){
						hashpq[tmp] = itl1;
					}
				}
			}
			else if(pq.size() < fnumber){	//not in the list, but no need to replace page
				miss++;
				pqpair *tmppq = new pqpair(tmp, 1, timestamp);
				pq.push_front(*tmppq);
				delete tmppq;
				for(itl1 = pq.begin(); itl1 != pq.end(); itl1++){
					itl2 = (itl1++);
					itl1--;
					if(*itl1 > *itl2){
						pqpair pqtmp = *itl2;
						itl1 = itl2;
						*itl2 = pqtmp;
					}
					if(itl1->page == tmp){
						hashpq[tmp] = itl1;
					}
				}
			}
			else{							//not in the list, and need to replace page
				miss++;
				if(hashall[tmp] > pq.front()){
					hashall[pq.front().page].cnt = 0;
					hashall[pq.front().page].time = 0;
					hashpq.erase(pq.front().page);
					pq.pop_front();
					pq.push_front(hashall[tmp]);
					for(itl1 = pq.begin(); itl1 != pq.end(); itl1++){
						itl2 = (itl1++);
						itl1--;
						if(*itl1 > *itl2){
							pqpair pqtmp = *itl2;
							itl1 = itl2;
							*itl2 = pqtmp;
						}
						if(itl1->page == tmp){
							hashpq[tmp] = itl1;
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