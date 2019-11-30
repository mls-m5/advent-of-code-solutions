/*
 * 13.cpp
 *
 *  Created on: 30 nov. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct Vec {
	int x, y;
};

enum Angle: char {
	None,
	Up,
	Right,
	Down,
	Left,
	Crashed = -100,
};


map <char, Angle> angleLookup {
	{'\0', None},
	{'^', Up},
	{'>', Right},
	{'v', Down},
	{'<', Left},
};


map <Angle, char> cartLookup {
	{None,   ' '},
	{Up,     '^'},
	{Right,  '>'},
	{Down,   'v'},
	{Left,   '<'},
	{Crashed,'x'},
};


class Tracks {
	struct Cart {
		Cart() = default;
		Cart(char c) {
			dir = angleLookup.at(c);
		}

		Angle dir = None;
		char moveNum = 0;
		char moved = 0;

		Vec direction() {
			switch (dir) {
			case Left:
				return {-1, 0};
			case Right:
				return {1, 0};
			case Up:
				return {0, -1};
			case Down:
				return {0, 1};
			case Crashed:
				return {0, 0};
			default:
				throw runtime_error("unknown cart direction");
			}
		}

		void rotateLeft() {
			if (--(char&)dir < 1) {
				(char&)dir += 4;
			}
		}

		void rotateRight() {
			if (++(char&)dir > 4) {
				(char&)dir -= 4;
			}
		}

		void rotate(char track) {
			switch (track) {
			case '/':
				switch (dir) {
				case Up:
				case Down:
					rotateRight();
					break;
				case Left:
				case Right:
					rotateLeft();
					break;
				default:
					break;
				}
				break;
			case '\\':
				switch (dir) {
				case Up:
				case Down:
					rotateLeft();
					break;
				case Left:
				case Right:
					rotateRight();
					break;
				default:
					break;
				}
				break;
			case '+':
				switch (moveNum) {
				case 0:
					rotateLeft();
					break;
				case 1:
					// Nothing
					break;
				case 2:
					rotateRight();
					break;
				default:
					break;
				}
				moveNum = (moveNum + 1) % 3;
			}
		}
	};

public:
	Tracks(string filename) {
		ifstream file(filename);

		vector <string> lines;

		for (string line; getline(file, line);) {
			lines.push_back(line);
		}

		_width = lines.front().size();
		_height = lines.size();

		_data.resize(_width * _height);
		_carts.resize(_data.size());

		int y = 0;
		for (const auto &line: lines) {
			for (int x = 0; x < line.size(); ++x) {
				auto c = line[x];
				switch (c) {
				case '<':
				case '>':
					cartAt(x, y) = Cart(c);
					c = '-';
					break;
				case '^':
				case 'v':
					cartAt(x, y) = Cart(c);
					c = '|';
					break;
				default:
					break;
				}
				at(x, y) = c;
			}
			++y;
		}
	}

	// Returns true on error
	bool moveCart(int x, int y) {
		auto &cart = cartAt(x, y);
		if (cart.dir && !cart.moved && cart.dir != Crashed) {
			auto direction = cart.direction();
			auto toX = x + direction.x;
			auto toY = y + direction.y;
			auto &newCartSlot = cartAt(toX, toY);
			if (newCartSlot.dir) {
				cout << "collision at " << toX << "," << toY << endl;
				newCartSlot.dir = Crashed;
				newCartSlot.moved = true;
				cart = {None};
				return true;
			}
			else {
				newCartSlot = cart;
				newCartSlot.moved = true;
				newCartSlot.rotate(at(toX, toY));
				cart = {None};
			}
		}

		return false;
	}

	bool tick() {
		bool isCrash = false;
		for (int y = 0; y < _height; ++y) {
			for (int x = 0; x < _width; ++x) {
				if (moveCart(x, y)) {
					isCrash = true;
				}
			}
		}
		for (auto &cart: _carts) {
			cart.moved = false;
		}
		return isCrash;
	}

	void print(bool includeCarts = true) {
		for (int y = 0; y < _height; ++y) {
			for (int x = 0; x < _width; ++x) {
				auto t = at(x, y);
				if (includeCarts) {
					auto c = cartAt(x, y);
					if (c.dir) {
						t = cartLookup.at(c.dir);
					}
				}
				cout << t;
			}
			cout << endl;
		}
	}

private:
	char &at(int x, int y) {
		return _data.at(x + y * _width);
	}

	Cart &cartAt(int x, int y) {
		return _carts.at(x + y * _width);
	}

	int _width, _height;
	vector<char> _data;
	vector<Cart> _carts;
};


int main(int argc, char **argv) {
	bool isTest = false;
	if (argc > 1 && string(argv[1]) == "--test") {
		isTest = true;
	}

	const string filename = isTest? "13ex.txt": "13.txt";

	Tracks tracks(filename);

	tracks.print(false);
	tracks.print();

	bool running = true;

	for (int i = 0; running; ++i) {
		if (tracks.tick()) {
			cout << "crash" << endl;
			running = false;
		}
		if (!isTest)tracks.print();
	}
}



