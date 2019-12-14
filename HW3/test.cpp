#include<iostream>
#include <time.h>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;
int main() {
	int num;
	cout << "Enter the num : ";
	cin >> num;
	fstream file;
	file.open("example.txt", ios::out);
	file << num << " ";
	for (int i = 0; i < num; i++) {
		file << rand() % num << " ";
	}
	file.close();
	return 0;
}