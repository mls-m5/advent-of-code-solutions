/*
 * 8.cpp
 *
 *  Created on: 8 dec. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

struct Canvas {

	Canvas (int w, int h, const string& input): width(w), height(h), depth(input.size() / w / h) {
		data.resize(input.size());
		transform(input.begin(), input.end(), data.begin(),
				[] (auto in) { return in - '0'; });
	}

	int &operator ()(int x, int y, int z) {
		return at(x, y, z);
	}
	int &at (int x, int y, int z) {
		return data.at(x + y * width + z * width * height);
	}

	int pixel(int x, int y) {
		for (int z = 0; z < depth; ++z) {
			auto d = at(x, y, z);
			if (d == 2) {
				continue;
			}
			else {
				return d;
			}
		}
		return 2;
	}

	void print() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				auto p = pixel(x, y);
				if (p == 2) {
					cout << " ";
				}
				else if (p == 1) {
					cout << "O";
				}
				else {
					cout << "·";
				}
			}
			cout << endl;
		}
	}

	int task1() {
		int minZeros = numeric_limits<int>::max();
		int minLayer = -1;

		for (int z = 0; z < depth; ++z) {
			int numZeros = 0;
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					if ((*this)(x, y, z) == 0) {
						++numZeros;
					}
				}
			}
			if (numZeros < minZeros) {
				minZeros = numZeros;
				minLayer = z;
			}
		}

		vector<int> count(10);
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				++ count.at(at(x, y, minLayer));
			}
		}

		return count.at(1) * count.at(2);
	}

	vector<int> data;
	int width, height, depth;
};

int main(int argc, char **argv) {
	const bool isTest = (argc > 1 && argv[1] == "--test"s);

	const int width = isTest? 3: 25;
	const int height = isTest? 2: 6;

	ifstream file(isTest? "data/8.ex.txt": "data/8.txt");

	string inputStr;
	file >> inputStr;

	Canvas canvas(width, height, inputStr);

	cout << "anwer 1: " << canvas.task1() << endl;

	canvas.print();
}




