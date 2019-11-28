/*
 * 3.cpp
 *
 *  Created on: 26 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <sstream>
#include <tuple>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

const bool isTest = false;

istringstream testFile(R"_(
#1 @ 1,3: 4x4
#2 @ 3,1: 4x4
#3 @ 5,5: 2x2
)_");


struct ElfRequest {
	int index;
	int x, y;
	int w, h;

	int *begin() {
		return &index;
	}
	int *end() {
		return &h + 1;
	}
};

struct Fabric {
	Fabric(int w, int h) {
		this->w = w;
		this->h = h;
		content.resize(w * h);
		fill(content.begin(), content.end(), pair<int, int>{0, 0});
	}

	auto &operator () (int x, int y) {
		if (x >= 0 && x < w && y >= 0 && y < h) {
			return content.at(y * w + x);
		}
		else {
			throw out_of_range("duh!");
		}
	}

	void print() {
		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < w; ++x) {
				cout << (*this)(x, y).first;
			}
			cout << endl;
		}
	}

	auto begin() {
		return content.begin();
	}

	auto end() {
		return content.end();
	}

	// Count, index
	vector<pair<int, int>> content;
	int w, h;
};


vector<ElfRequest> parseRequests(istream &stream) {
	vector<ElfRequest> requests;
	for (string line; getline(stream, line);) {
		if (line.empty()) {
			continue;
		}

		regex r("[0-9]");
		auto b = sregex_iterator(line.begin(), line.end(), r);
		auto e = sregex_iterator();
		if (distance(b, e) != 5) {
			throw runtime_error("wrong number of numbers found");
		}
		ElfRequest request;
		transform(
				b,
				e,
				request.begin(),
				[] (auto s) ->int {
			return stoi(s.str());
		});

		requests.push_back(request);

	}
	return requests;
}

void printRequests(const vector<ElfRequest> &requests) {
	for (auto r: requests) {
		cout << "#" << r.index << " " << r.x << "," << r.y << " " << r.w << "x" << r.h << endl;
	}
}


void task3(istream &stream) {
	auto requests = parseRequests(stream);

	int maxWidth = 0;
	int maxHeight = 0;

	if (isTest) {
		printRequests(requests);
	}

	for (auto request: requests) {
		maxWidth = max(maxWidth, request.x + request.w + 1);
		maxHeight = max(maxHeight, request.y + request.h + 1);
	}

	Fabric fabric(maxWidth, maxHeight);

	cout << "Fabric is at least " << maxWidth << "x" << maxHeight << "''" << endl;

	for (auto request: requests) {
		for (int y = 0; y < request.h; ++y) {
			for (int x = 0; x < request.w; ++x) {
				auto &f = fabric(x + request.x, y + request.y);
				++f.first;
				if (f.second != 0) {
					f.second = -1;
				}
				else {
					f.second = request.index;
				}
			}
		}
	}

//	if (isTest) {
		fabric.print();
//	}

	int twoOrMore = 0;
	for (auto value: fabric) {
		if (value.first >= 2) {
			++twoOrMore;
		}
	}

	cout << "Two or more elves wants " << twoOrMore << " square inches of fabric" << endl;


	for (auto request: requests) {
		auto isTheOnlyOne = true;
		for (int y = 0; y < request.h; ++y) {
			for (int x = 0; x < request.w; ++x) {
				auto &f = fabric(x + request.x, y + request.y);
				if (f.second != request.index) {
					isTheOnlyOne = false;
					break;
				}
			}
			if (!isTheOnlyOne) {
				break;
			}
		}
		if (isTheOnlyOne) {
			cout << "the only free request is " << request.index << endl;
//			break;
		}
	}
}


int main(int argc, char **argv) {
	if (isTest) {
		task3(testFile);
	}
	else {
		ifstream file("3.txt");
		task3(file);
	}
}


