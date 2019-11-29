/*
 * 8.cpp
 *
 *  Created on: 29 nov. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>

using namespace std;

char lastName = 'A' - 1;

struct Node {
	Node(istream &stream) {
		int numChildren;
		int numMetadata;
		name = ++lastName;

		stream >> numChildren;
		stream >> numMetadata;

		for (int i = 0; i < numChildren; ++i) {
			children.emplace_back(stream);
		}

		for (int i = 0; i < numMetadata; ++i) {
			int data;
			stream >> data;
			metadata.push_back(data);
		}
	}

	int sumMetadata() {
		int sum = 0;
		for (auto &c: children) {
			sum += c.sumMetadata();
		}
		for (auto m: metadata) {
			sum += m;
		}
		return sum;
	}

	int value() {
		if (children.empty()) {
			return accumulate(metadata.begin(), metadata.end(), 0);
		}
		else {
			int sum = 0;
			for (auto m: metadata) {
				auto index = m - 1;
				try {
					auto child = children.at(index);
					sum += child.value();
				}
				catch (out_of_range &) {
					// ...
				}
			}
			return sum;
		}
	}

	void print(int level = 0) const {
		auto indent = [level] () {
			for (int i = 0; i < level; ++i) cout << "┣━";
		};

		indent();
		cout << name << endl;

		for (const auto &c: children) {
			c.print(level + 1);
		}
		for (auto m: metadata) {
			indent();
			cout << m << endl;
		}

	}

	vector<Node> children;
	vector<int> metadata;
	char name;
};

int main(int argc, char **argv) {
	bool isTest = false;
	if (argc > 1 && string(argv[1]) == "--test") {
		isTest = true;
	}

	const string filename = (isTest)? "8ex.txt": "8.txt";
	ifstream file(filename);

	Node root(file);

	if (isTest) root.print();

	cout << "answer 1: " << root.sumMetadata() << endl;
	cout << "answer 2: " << root.value() << endl;
}




