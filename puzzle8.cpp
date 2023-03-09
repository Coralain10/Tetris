#pragma once

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <stack>
#include <vector>
#define ushort unsigned short
#define uint unsigned int
#define puis pair<uint, ushort*>
using namespace std;

class Puzzle8 {
	ushort width;
	ushort height;
	uint totalSize;
	ushort *matrixOk;

	bool isSolved(ushort *m) {
		for (uint i = 0; i < totalSize; i++) {
			if (m[i] != matrixOk[i]) {
				return false;
			}
		}
		return true;
	}

	void print(ushort* m) {
		ushort col = 0;
		for (uint i = 0; i < totalSize; i++)
		{
			if (i != 0 && i % width == 0) {
				col++;
				cout << endl;
			}
			cout << m[i] << " ";
		}
		cout << endl;
	}

	void solve(ushort* m, uint emptyI) {
		stack<puis> st;
		st.push({ emptyI, m });
		vector<puis> steps;
		ushort* current;
		while (st.size() > 0)
		{
			current = st.top().second;
			emptyI = st.top().first;

			if (isSolved(current)) {
				cout << "\nSOLUCION\n";
				print(current);
				return;
			}
			else {
				//cout << emptyI << "\t";
				vector<puis> newms;
				if (emptyI > width) {
					//swap up
					ushort* newm = current;
					swap(newm[emptyI - width], newm[emptyI]);
					newms.push_back({ emptyI - width, newm });
				}
				if (emptyI < totalSize - width) {
					//swap down
					ushort* newm = current;
					swap(newm[emptyI + width], newm[emptyI]);
					newms.push_back({ emptyI + width, newm });
				}
				if (emptyI % width > 0) {
					//swap left
					ushort* newm = current;
					swap(newm[emptyI - 1], newm[emptyI]);
					newms.push_back({ emptyI - 1, newm });
				}
				if (emptyI % width < width - 1) {
					//swap right
					ushort* newm = current;
					swap(newm[emptyI + 1], newm[emptyI]);
					newms.push_back({ emptyI + 1, newm });
				}
				random_shuffle(newms.begin(), newms.end());
				for (auto &n : newms) st.push(n);
			}
		}
	}

public:
	Puzzle8(ushort width, ushort height) :
		width(width), height(height) {
		if (width < 3) width = 3;
		if (height < 3) height = 3;
		totalSize = height * width;
		matrixOk = new ushort;
	}
	/*~Puzzle8() {
		delete matrix;
	}*/

	void run() {
		ushort* matrix = new ushort;
		for (uint i = 0; i < totalSize; i++) {
			matrixOk[i] = matrix[i] = i;
		}
		random_shuffle(matrix, matrix + totalSize);
		print(matrix);
		
		//find(matrix, matrix + totalSize, totalSize-1);
		uint emptyI = -1;
		for (uint i = 0; i < totalSize; i++) {
			if (matrix[i] == totalSize-1) {
				emptyI = i;
				break;
			}
		}

		solve(matrix, emptyI);
	}
};


/*int main() {
	srand(time(NULL));

	Puzzle8 p8(3,3);
	p8.run();

	cin.get();
	cin.ignore();
	return 0;
}*/