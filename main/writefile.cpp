#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include "writefile.h"
using namespace std;

#define prl cout<<endl


void Writefile::write(string filename) {
	int mode, cnt;
	cout << ">>> Choose 1 for linear, 2 for random: "; cin >> mode;
	cout << ">>> Input number of numbers: "; cin >> cnt;

	ofstream file;
	file.open(filename);

	if (mode == 1) {
		for (int i = 0; i < cnt; ++i)
			file << i << " ";
	} else if (mode == 2) {
		int lo, hi;
		cout << ">>> Input lower and upper bound, separate with \'enter\': ";
		cin >> lo >> hi;
		random_device rand;

		for (int i = 0; i < cnt; ++i) {
			mt19937 rng(rand());
			uniform_int_distribution<mt19937::result_type> dist6(lo, hi);
			file << dist6(rng) << " ";
		}
	}
	file.close();
}

void Writefile::writefile() {
	string filename;
	cout << ">>> Input file name: "; cin >> filename;
	write(filename);
}

void Writefile::writefiledftdir(string filename) {
	write(filename);
}

// FOR TESTING ONLY

int main() {
	Writefile wrf;
	wrf.writefile();
	return 0;
}	