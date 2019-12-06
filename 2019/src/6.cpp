/*
 * 6.cpp
 *
 *  Created on: 6 dec. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

struct Node {
	Node() = default;
	Node(Node &&) = default;
	Node(string name, string parent): name(name), parent(parent) {}
	Node &operator = (Node &&) = default;

	void add(Node &&child) {
		children.push_back(move(child));
	}

	Node *find(const string &name) {
		if (name.empty()) {
			return nullptr;
		}
		if (this->name == name) {
			return this;
		}
		for (auto &child: children) {
			if (auto found = child.find(name)) {
				return found;
			}
		}
		return nullptr;
	}

	void print(int level = 0) {
		for (int i = 0; i < level; ++i) {
			cout << "-";
		}
		cout << name << "(" << parent << ") = " << sum(level) << endl;
		for (auto &child: children) {
			child.print(level + 1);
		}
	}

	int sum(int level = 0) const {
		int sum = level + 1;

		for (const auto &child: children) {
			sum += child.sum(level + 1);
		}

		return sum;
	}

	vector <Node> children;
	string name;
	string parent;
};

Node loadNodes(const string &filename) {
	vector <Node> nodes;
	Node root;

	ifstream file(filename);

	for (string line; getline(file, line);) {
		istringstream ss(line);
		string parent, name;
		getline(ss, parent, ')');
		getline(ss, name, ')');

		nodes.emplace_back(name, parent);
		if (nodes.back().parent == "COM") {
			root = move(nodes.back());
			nodes.pop_back();
		}
	}

	for (auto &node: nodes) {
		cout << "searching for parent for node " << node.name << endl;
		for (auto &parent: nodes) {
			if (parent.name == node.parent) {
				parent.add(move(node));
				break;
			}
			else if (!parent.name.empty()) {
				if (auto found = parent.find(node.parent)) {
					found->add(move(node));
					break;
				}
			}
		}
		if (node.name.empty()) {
			continue;
		}
		else {
			if (auto parent = root.find(node.parent)) {
				parent->add(move(node));
				continue;
			}
		}
		throw runtime_error("could not find parent");
	}

	return root;
}

int main(int argc, char **argv) {
	const bool isTest = argc > 1 && string(argv[1]) == "--test";

	auto root = loadNodes(isTest? "data/6.ex.txt": "data/6.txt");

	root.print();

	cout << "answer 1: the sum is " << root.sum() << endl;
}




