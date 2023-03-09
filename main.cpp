//#include "tetris.hpp"
//#include "puzzle8.hpp"
#include <iostream>
#include <string>
using namespace std;

string games[] = {"Tetris", "Puzzle 8"};

//int main() {
//	cout << "ELIGE UN EJERCICIO \n";
//	for (size_t i = 0; i < 2; i++)
//		cout << i << ":" << games[i];
//
//	char c;
//	int idx = -1;
//	do
//	{
//		if (toupper(c) == 'Q') break;
//
//		idx = c - '0';
//		if (idx >= 0 && idx < games->size())
//		{
//			switch (idx)
//			{
//			case 0:
//				Tetris t;
//				t.run();
//				break;
//			case 1:
//				Puzzle8 p;
//				p.run();
//				break;
//			}
//		}
//	} while ( cin.get(c) );
//
//	cin.get();
//	cin.ignore();
//}