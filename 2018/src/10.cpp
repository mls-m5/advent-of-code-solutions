/*
 * 10.cpp
 *
 *  Created on: 29 nov. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

struct Rect {
	int x1, y1, x2, y2;
};

struct Point {
public:
	Point(string line) {
		regex r("-?[0-9]+");

		transform(
				sregex_iterator(line.begin(), line.end(), r),
				sregex_iterator(),
				&x, [](auto value) {
			return stoi(value.str());
		});
	}

	void step() {
		x += vx;
		y += vy;
	}

	int x = 0, y = 0, vx = 0, vy = 0;
};

vector<Point> loadPoints(string filename) {
	vector<Point> ret;
	ifstream file(filename);
	for (string line; getline(file, line);) {
		ret.emplace_back(line);
	}
	return ret;
}

Rect getDimensions(const vector<Point>& points) {
	Rect rect = {100000, 100000, -100000, -100000};

	for (auto point: points) {
		rect.x1 = min(point.x, rect.x1);
		rect.y1 = min(point.y, rect.y1);
		rect.x2 = max(point.x, rect.x2);
		rect.y2 = max(point.y, rect.y2);
	}

	return rect;
}

struct Canvas {
	Canvas(Rect dims):
	dims(dims){
		width = dims.x2 - dims.x1 + 1;
		height = dims.y2 - dims.y1 + 1;
		image.resize(width * height);
	}

	int &at(int x, int y) {
		return image.at((x - dims.x1) + (y - dims.y1) * width);
	}

	void print() {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				auto pixel = image.at(x + y * width);
				if (pixel) {
					cout << "#";
				}
				else {
					cout << "." ;
				}
			}
			cout << endl;
		}
	}

	void draw(const Point &point) {
		try {
			at(point.x, point.y) = 1;
		}
		catch (out_of_range &) {
			// ..
		}
	}

	void clear() {
		for (auto &pixel: image) {
			pixel = 0;
		}
	}

private:
	int width;
	int height;
	Rect dims;
	vector<int> image;
};

// Used to check if the points is in place
bool isConnected(const vector <Point> &points) {
	for (int i = 0; i < points.size(); ++i) {
		bool connected = false;
		for (int j = 0; j < points.size(); ++j) {
			if (i != j) {
				auto point1 = points[i];
				auto point2 = points[j];
				if (abs(point1.x - point2.x) <= 2 && abs(point1.y - point2.y) <= 2) {
					connected = true;
				}
			}
		}
		if (!connected) {
			return false;
		}
	}
	return true;
}


int main(int argc, char **argv) {
	bool isTest = false;
	if (argc > 1 && string(argv[1]) == "--test") {
		isTest = true;
	}

	auto points = loadPoints(isTest? "10ex.txt": "10.txt");

	for (int i = 0; i < 100000; ++i) {
		for (auto &point: points) {
			point.step();
		}

		auto dimensions = getDimensions(points);

		if (dimensions.x2 - dimensions.x1 > 1000) {
			continue;
		}
		if (dimensions.y2 - dimensions.y1 > 1000) {
			continue;
		}
		if (!isConnected(points)) {
			continue;
		}

		Canvas canvas(dimensions);

		for (auto &point: points) {
			canvas.draw(point);
		}
		canvas.print();
		cout << endl;
		cout << "answer 2: " << i + 1 << endl;
		return 0;
	}
}



