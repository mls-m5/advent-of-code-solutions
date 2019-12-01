
#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>
#include <iterator>

using namespace std;

int fuel (int mass, bool recurse = false) {
	int fuel = mass / 3 - 2; // int automatically rounds down

	if (recurse && fuel > 0) {
		fuel += ::fuel(fuel, true);
	}

	return fuel > 0? fuel: 0;
}

int totalFuel(const vector<int> &masses, bool recurse) {
	int total = 0;
	for (auto mass: masses) {
		total += fuel(mass, recurse);
	}
	return total;
}


int main(int argc, char **argv) {
	const bool isTest = argc > 1 && string(argv[1]) == "--test";

	ifstream file(isTest? "data/1.ex.txt": "data/1.txt");
	const vector <int> masses (
			(istream_iterator<int>(file)),
			istream_iterator<int>());

	// Part 1
	cout << "answer 1 " << totalFuel(masses, false) << endl;

	// Part 2
	cout << "answer 2 " << totalFuel(masses, true) << endl;

	return 0;
}

