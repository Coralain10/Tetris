#pragma once

#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

#define uint unsigned int
#define ush unsigned short
const uint HEIGHT = 16;
const uint WIDTH = 8;

enum class TS {FREE, FULL, MOVING};
TS matrix[HEIGHT][WIDTH]; //**matrix

struct Vec2 {
	int x;
	int y;
	Vec2() {}
	Vec2(int x, int y) : x(x), y(y) {}
};

const Vec2 vecZero(0, 0);
const Vec2 vecDown(0, 1);
const Vec2 vecRight(1, 0);
const Vec2 vecLeft(-1, 0);

class Tetris {

	class Block {
		Vec2 parts[4]; //relative position
		Vec2 origin;
		bool endFalling = false;
		ush width, height;

		Vec2 getPartGlobalPos(Vec2 part, Vec2 offset = vecZero) {
			return Vec2(
				origin.x + part.x + offset.x,
				origin.y + part.y + offset.y
			);
		}
		void clean() {
			Vec2 nx;
			for (Vec2& p : parts) {
				nx = getPartGlobalPos(p);
				matrix[nx.y][nx.x] = TS::FREE;
			}
		}
		void paint() {
			Vec2 nx;
			for (Vec2& p : parts) {
				nx = getPartGlobalPos(p);
				matrix[nx.y][nx.x] = TS::MOVING;
			}
		}
		void move(Vec2 offset) {
			clean();
			origin.x += offset.x;
			origin.y += offset.y;
			paint();
		}
		void checkEndFalling(Vec2 offset = vecZero) {
			Vec2 nx;
			for (Vec2& p : parts) {
				nx = getPartGlobalPos(p, offset);
				if (nx.y == HEIGHT || matrix[nx.y][nx.x] == TS::FULL) {
					endFalling = true;
					break;
				}
			}
			if (endFalling) {
				for (Vec2& p : parts) {
					nx = getPartGlobalPos(p);
					matrix[nx.y][nx.x] = TS::FULL;
				}
			}
		}

	public:
		Block() {}
		Block(Vec2* parts) {
			width = 0, height = 0;

			for (size_t i = 0; i < 4; i++) {
				if (parts[i].x > width) width = parts[i].x;
				if (parts[i].y > height) height = parts[i].y;
			}
			origin = Vec2(width / 2, height / 2);

			for (size_t i = 0; i < 4; i++) {
				parts[i].x -= origin.x;
				parts[i].y -= origin.y;
				this->parts[i] = parts[i];
			}

			origin.x += WIDTH / 2;
		}

		void fall() {
			//cout << "origin " << origin.x << ", " << origin.y << endl;
			checkEndFalling(vecDown);
			if (!endFalling) move(vecDown);
		}

		void moveSides(bool toRight) {
			Vec2 nx;
			for (Vec2& p : parts) {
				nx = getPartGlobalPos(p, toRight ? vecRight : vecLeft);
				if ((toRight && nx.x == WIDTH) ||
					(!toRight && nx.x < 0) ||
					matrix[nx.y][nx.x] == TS::FULL)
					return;
			}
			move(toRight ? vecRight : vecLeft);
		}

		void rotate() {
			Vec2 aux[4];
			short q = 0;
			for (Vec2 p : parts) {
				aux[q] = p;
				aux[q].y *= -1;
				swap(aux[q].x, aux[q].y);
				p = getPartGlobalPos(aux[q]);
				if (p.x <= 0 || p.y <= 0 ||
					p.x >= WIDTH || p.y >= HEIGHT ||
					matrix[p.y][p.x] == TS::FULL)
					break;
				q++;
			}
			if (q >= 4) {
				clean();
				q = 0;
				for (Vec2& p : parts) {
					p = aux[q];
					q++;
				}
				swap(width, height);
				paint();
				checkEndFalling();
			}
		}

		bool didStop() {
			return endFalling;
		}

		pair<int, int> getMinMaxYs() {
			int miny = 10, maxy = 0;
			for (Vec2& p : parts) {
				if (p.y > maxy) maxy = p.y;
				if (p.y < miny) miny = p.y;
			}
			return { miny + origin.y, maxy + origin.y };
		}
	};

private:
	Block blockTypes[7];
	uint score;
	bool isAlive;

	void printMatrix() {
		for (uint i = 0; i < HEIGHT + 2; i++)
		{
			for (uint j = 0; j < WIDTH + 2; j++)
			{
				if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1)
					cout << "+";
				else
				{
					switch (matrix[i - 1][j - 1])
					{
					case TS::FREE: cout << ' '; break;
					case TS::MOVING: cout << 'x'; break;
					case TS::FULL: cout << '#'; break;
					}
				}
				cout << " ";
			}
			cout << endl;
		}
	}

	void checkIfFree(Block& currBlock) {
		pair<int, int> cys = currBlock.getMinMaxYs();
		if (cys.first == 0) {
			isAlive = false;
			return;
		}
		for (ush i = cys.first; i <= cys.second; i++)
		{
			bool qfull = true;
			for (size_t j = 0; j < WIDTH; j++)
			{
				if (matrix[i][j] == TS::FREE) {
					qfull = false;
					break;
				}
			}
			if (qfull) {
				score++;
				/*for (size_t j = 0; j < WIDTH; j++)
					matrix[i][j] = TS::FREE;*/
				for (size_t j = i; j > 0; j--)
					for (size_t k = 0; k < WIDTH; k++)
						matrix[j][k] = matrix[j - 1][k];
				for (size_t j = 0; j < WIDTH; j++)
					matrix[0][j] = TS::FREE;
			}
		}
	}

public:
	Tetris():score(0), isAlive(true){
		srand(time(NULL));

		Vec2 bTS[7][4] = {
			{Vec2(0,0), Vec2(0,1), Vec2(0,2), Vec2(0,3)}, // |
			{Vec2(0,0), Vec2(1,0), Vec2(0,1), Vec2(1,1)}, // []
			{Vec2(1,0), Vec2(0,1), Vec2(1,1), Vec2(2,1)}, // _|_
			{Vec2(0,0), Vec2(1,0), Vec2(1,1), Vec2(2,1)}, // -|_
			{Vec2(1,0), Vec2(2,0), Vec2(0,1), Vec2(1,1)}, // _|-
			{Vec2(0,0), Vec2(0,1), Vec2(0,2), Vec2(1,2)}, // |.
			{Vec2(1,0), Vec2(1,1), Vec2(1,2), Vec2(0,2)} // .|
		};

		for (ush i = 0; i < 7; i++)
			blockTypes[i] = Block(bTS[i]);
	}
	void run() {
		char currKey = 'S';
		Block currBlock = blockTypes[rand() % 7];
		while (isAlive && currKey != 'Q') {
			cout << "LINEAS HECHAS: " << score << endl;
			printMatrix();

			if (currBlock.didStop()) {
				checkIfFree(currBlock);
				currBlock = blockTypes[rand() % 7];
			}
			currBlock.fall();

			cin >> currKey;
			currKey = toupper(currKey);
			switch (currKey)
			{
			case 'A': currBlock.moveSides(false); break;
			case 'D': currBlock.moveSides(true); break;
			case 'X': currBlock.rotate(); break;
			}
		};

		cout << "PUNTAJE FINAL: " << score << endl;
	}
};





int main() {

	Tetris t;
	t.run();

	// cin.get();
	// cin.ignore();
	return 0;
}