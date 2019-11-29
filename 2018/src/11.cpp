/*
 * 11.cpp
 *
 *  Created on: 29 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <thread>
#include <mutex>
#include <list>

using namespace std;

// Much faster than do a to_string apparently
int hundredth(int value) {
	if (value < 100) {
		return 0;
	}
	value = value / 100;
	auto tmpValue = value / 10;
	value -= tmpValue * 10;
	return value;
}

int powerLevel(int x, int y, int serialNumber) {
	const auto rackID = x + 10;
	auto powerLevel = rackID * y;
	powerLevel += serialNumber;
	powerLevel *= rackID;
//	auto stringValue = to_string(powerLevel);
//	if (stringValue.size() < 3) {
//		powerLevel = 0;
//	}
//	else {
//		powerLevel = stringValue[stringValue.size() - 3] - '0';
//	}
	powerLevel = hundredth(powerLevel);
	powerLevel -= 5;
	return powerLevel;
}

struct ExampleValues {
	int x, y, serialnumber, level;
};

int evaluateSquare(int sx, int sy, int serialNumber, int size) {
	int sum = 0;
	for (int y = 0; y < size; ++y) {
		for (int x = 0; x < size; ++x) {
			sum += powerLevel(x + sx, y + sy, serialNumber);
		}
	}
	return sum;
}

tuple<int, int, int> findMax(int serialNumber, int size = 3) {
	const int width = 300;
	const int height = 300;
	int max = 0;
	int maxX = 0, maxY = 0;
	for (int y = 1; y <= width - size + 1; ++y) {
		for (int x = 1; x <= height - size + 1; ++x) {
			auto value = evaluateSquare(x, y, serialNumber, size);
			if (value > max) {
				maxX = x;
				maxY = y;
				max = value;
			}
		}
	}
	return {maxX, maxY, max};
}

tuple<int, int, int, int> findMaxDifferentSizes(int serialNumber) {
	int max = 0;
	int maxX = 0, maxY = 0;
	int selectedSize = 0;
	mutex changeMutex;
	const int numParts = thread::hardware_concurrency() * 4;
//	const int numParts = 1;
	int progress; // Ints ar atomic

	list<thread> threads;
	for (int part = 0; part < numParts; ++part) {
		int begin = 1 + 300 * part / numParts;
		int end = 1 + 300 * (part + 1) / numParts;
		threads.emplace_back([serialNumber, &selectedSize, &maxX, &maxY, &max, &changeMutex, &progress](int begin, int end) {
			for (int size = begin; size < end; ++size) {
				int x, y, power;
				tie(x, y, power) = findMax(serialNumber, size);
				lock_guard<mutex> g(changeMutex);
				if (power > max) {
					maxX = x;
					maxY = y;
					max = power;
					selectedSize = size;
				}
				++progress;
				if (progress % 10 == 0) {
					cout << 100 * progress / 300 << "%" << endl;
				}
			}
			}, begin, end);
	}

	for (auto &thread: threads) {
		thread.join();
	}
	return {maxX, maxY, max, selectedSize};
}


void doExamples() {
	vector<ExampleValues> examples {
		{3, 5, 8, 4},
		{122, 79, 57, -5},
		{217, 196, 39, 0},
		{101, 153, 71, 4},
	};

	for (auto example: examples) {
		auto level = powerLevel(example.x, example.y, example.serialnumber) ;
		cout << level << endl;
		if (level != example.level) {
			cout << "wrong value" << endl;
		}
	}

	int x, y, power, size;
	{
		tie(x, y, power) = findMax(18);
		cout << x << "," << y << " power " << power << endl;
//		cout << coord.first << "," << coord.second << endl;
	}

	{
		tie(x, y, power) = findMax(42);
		cout << x << "," << y << " power " << power << endl;
	}

	{
//		tie(x, y, power) = findMax(18, 16);
//		cout << x << "," << y << " power " << power << endl;
		if (false) {
			tie(x, y, power, size) = findMaxDifferentSizes(18);
			cout << x << "," << y << "," << size << " power " << power << endl;
		}
	}
}

int main(int argc, char **argv) {
	doExamples();

	int x, y, power, size;
	tie(x, y, power) = findMax(1309);
	cout << "answer 1: " << x << "," << y << " power " << power << endl;

	tie(x, y, power, size) = findMaxDifferentSizes(1309);
	cout << "answer 2: " << x << "," << y << "," << size << " power " << power << endl;
}


