/*
 * 3.cpp
 *
 *  Created on: 2 dec. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;
struct Vec {
	int x = 0, y = 0;

	Vec() = default;
	Vec(const Vec &other) = default;
	Vec(int x, int y):
		x(x), y(y) {}

	Vec(string init): Vec() {
		auto value = stoi(init.substr(1));
		switch (init[0]) {
		case 'R':
			x = value;
			break;
		case 'L':
			x = -value;
			break;
		case 'D':
			y = value;
			break;
		case 'U':
			y = -value;
			break;
		}
	}

	int manhattan(const Vec &other) const {
		return abs(other.x - x) + abs(other.y - y);
	}

	int manhattan() const {
		return abs(x) + abs(y);
	}

	Vec &operator/=(int value) {
		x /= value;
		y /= value;
		return *this;
	}

	Vec operator -(const Vec &other) const  {
		return {x - other.x, y - other.y};
	}

	Vec operator +(const Vec &other) const  {
		return {x + other.x, y + other.y};
	}

	Vec operator *(int value) const {
		return {x * value, y * value};
	}

	bool operator == (const Vec &other) const {
		return x == other.x && y == other.y;
	}
};



vector<Vec> loadCable(istream &file) {
	string line;
	getline(file, line);
	istringstream liness(line);
	vector<Vec> vecs;
	Vec previous;
	for (string word; getline(liness, word, ',');) {

		auto d = Vec(word);
		auto len = d.manhattan();
		d /= (len);
		for (int i = 1; i <= len; ++i) {
			vecs.push_back(previous + d * i);
		}
		previous = vecs.back();
	}
	return vecs;
}


struct Canvas {
    Canvas (int minx, int miny, int maxx, int maxy):
    minx(minx), miny(miny), maxx(maxx), maxy(maxy){
        width = maxx - minx + 1;
        height = maxy - miny + 1;

        data.resize(width * height);
    }

    int &operator() (int x, int y) {
        x -= minx;
        y -= miny;
        return data.at(x + y * width);
    }

    void print() {
    	for (int y = 0; y < height; ++y) {
    		for (int x = 0; x < width; ++x) {
    			if (auto d = data[x + y * width]) {
    				cout << setw(3) << d;
    			}
    			else {
    				cout << " . ";
    			}
    		}
    		cout << endl;
    	}
    }

    vector <int> data;
    int minx, miny, maxx, maxy;
    int width;
    int height;
};

int main(int argc, char **argv) {
	const bool isTest = argc > 1 && string(argv[1]) == "--test";

	ifstream file(isTest? "data/3.ex.txt": "data/3.txt");

	const auto cable1 = loadCable(file);
    const auto cable2 = loadCable(file);


    int minWidth = 100000;
	int maxWidth = 0;
	int maxHeight = 0;
    int minHeight = 10000;

	for (auto c: cable1) {
		maxWidth = max(maxWidth, c.x);
		minWidth = min(minWidth, c.x);
		maxHeight = max(maxHeight, c.y);
		minHeight = min(minHeight, c.y);
	}

    for (auto c: cable2) {
        maxWidth = max(maxWidth, c.x);
        minWidth = min(minWidth, c.x);
        maxHeight = max(maxHeight, c.y);
        minHeight = min(minHeight, c.y);
    }

    vector <pair<Vec, int>> collisions;

    Canvas canvas(minWidth, minHeight, maxWidth, maxHeight);

    auto index = 0;
    for (auto c: cable1) {
        canvas(c.x, c.y) = ++index;
    }

    // For debugging
    if (isTest) {
    	canvas.print();

    	Canvas canvas2(minWidth, minHeight, maxWidth, maxHeight);

    	{
    		auto index = 0;
    		for (auto c: cable2) {
    			canvas2(c.x, c.y) = ++index;
    		}
    		canvas2.print();
    	}


    	// Collision view
    	Canvas canvas3(minWidth, minHeight, maxWidth, maxHeight);

    	{
    		auto index = 0;
    		for (auto c: cable1) {
    			canvas3(c.x, c.y) += ++index;

    		}
    		index = 0;
    		for (auto c: cable2) {
    			canvas3(c.x, c.y) += ++index;
    		}
    		canvas3.print();
    	}
    }


    index = 1;
    for (auto c: cable2) {
        ++index;
        if (auto pixel = canvas(c.x, c.y)) {
            collisions.push_back({c, pixel + index});
        }
    }


    auto min = min_element(collisions.begin(), collisions.end(), [](auto a, auto b) {
        return a.first.manhattan() < b.first.manhattan();
    });

    cout << "answer 1: " << min->first.x << "," << min->first.y;
    cout << " --> " << min->first.manhattan() << endl;

    auto min2 = min_element(collisions.begin(), collisions.end(), [](auto a, auto b) {
        return a.second < b.second;
    });

    cout << "answer 2: " << min2->second - 1 << endl;
}





