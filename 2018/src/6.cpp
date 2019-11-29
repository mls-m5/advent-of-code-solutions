/*
 * 6.cpp
 *
 *  Created on: 29 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <cmath>

using namespace std;

struct Coord {
	int x, y;

	int manhattan(const Coord& other) const {
		return abs(other.x - x) + abs(other.y - y);
	}


	int totalDistanceTo(const vector<Coord> coords) {
		int sum = 0;
		for (auto coord: coords) {
			sum += this->manhattan(coord);
		}
		return sum;
	}

};

struct CoordMap {
	struct Element {
		int distance = 0;
		int totalDistance = 0;
		int coordId = -1;
	};

	CoordMap(int width, int height, vector<Coord> coordinates):
		data(width * height),
		width(width),
		height(height) {
		calculateDistances(coordinates);
	}

	Element &operator() (int x, int y) {
		return data.at(x + y * width);
	}

	Element &at(int x, int y) {
		return (*this)(x, y);
	}

	// Calculate distance from location to all coordinates
	static Element calculateLocationDistance(Coord from, const vector<Coord> &coords) {
		Element res;
		res.distance = numeric_limits<int>::max();
		for (int i = 0; i < coords.size(); ++i) {
			auto coord = coords[i];
			auto dist = coord.manhattan(from);
			if (dist < res.distance) {
				res.distance = dist;
				res.coordId = i;
			}
			else if (dist == res.distance) {
				res.coordId = -1;
			}
		}
		res.totalDistance = from.totalDistanceTo(coords);
		return res;
	}

	void calculateDistances(const vector<Coord> &coords) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				at(x, y) = calculateLocationDistance({x, y}, coords);
			}
		}
	}

	// Visualize answer 1
	void print() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				auto element = at(x, y);
				char c = 'a' + element.coordId;
				if (element.distance == 0) {
					c -= ('a' - 'A');
				}
				cout << ((element.coordId == -1)? '.': c);
			}
			cout << endl;
		}
	}

	// Visualize answer 2
	void printLessThan(int size) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				auto element = at(x, y);
				if (element.distance == 0 && element.coordId > -1) {
					cout << (char)(element.coordId + 'A');
				}
				else if (element.totalDistance < size) {
					cout << "#";
				}
				else {
					cout << ".";
				}
			}
			cout << endl;
		}
	}

	// Count all locations with less total distance than the given argument
	int areaLessThan(int size) {
		int area = 0;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				if (at(x, y).totalDistance < size) {
					++area;
				}
			}
		}
		return area;
	}

	// Find coordinates touching borders
	set<int> touchingBorders() {
		set<int> ret;

		for (int x = 0; x < width; ++x) {
			ret.insert(at(x, 0).coordId);
			ret.insert(at(x, height - 1).coordId);
		}

		for (int y = 0; y < height; ++y) {
			ret.insert(at(0, y).coordId);
			ret.insert(at(width - 1, y).coordId);
		}

		return ret;
	}

	// Clear coordinates with infinite area
	void removeBorderTouching() {
		auto borderTouching = touchingBorders();

		for (auto &element: data) {
			if (borderTouching.find(element.coordId) != borderTouching.end()) {
				element.distance = 0;
				element.coordId = -1;
			}
		}
	}

	int largestArea() {
		map<int, int> areas;
		for (auto element: data) {
			++areas[element.coordId];
		}

		areas[-1] = 0;

		return max_element(areas.begin(), areas.end(), [] (auto a, auto b) {
			return a.second < b.second;
		})->second;
	}

	vector<Element> data;
	int width, height;

};

vector<Coord> readCoordinates(string filename) {
	ifstream input(filename);

	vector<Coord> coordinates;

	for (string word; input >> word;) {
		auto x = stoi(word.substr(0, word.size() - 1)); // Remove comma
		input >> word;
		auto y = stoi(word);
		auto v = Coord{x, y};
		coordinates.push_back(v);
	}

	return coordinates;
}

Coord getDimensions(const vector<Coord> &coordinates) {
	Coord max {0, 0};
	for (auto coord: coordinates) {
		max.x = std::max(coord.x, max.x);
		max.y = std::max(coord.y, max.y);
	}
	return max;
}


int main(int argc, char **argv) {
	string inputFilename = "6.txt";
	bool isTest = false;
	if (argc > 1 && string(argv[1]) == "--test") {
		inputFilename = "6ex.txt";
		isTest = true;
	}

	const auto coordinates = readCoordinates(inputFilename);

	const auto dimensions = getDimensions(coordinates);

	{
		// Part 1
		CoordMap coordMap(dimensions.x + 1, dimensions.y + 1, coordinates);

		if (isTest) coordMap.print();

		coordMap.removeBorderTouching();

		if (isTest) coordMap.print();

		cout << "answer 1: largest area = " << coordMap.largestArea() << endl;
	}

	{
		// Part 2
		CoordMap coordMap(dimensions.x + 1, dimensions.y + 1, coordinates);

		int limit = isTest? 32: 10000;

		if (isTest) coordMap.printLessThan(limit);

		cout << "answer 2: area smaller than " << limit << " --> " << coordMap.areaLessThan(limit) << endl;
	}
}



