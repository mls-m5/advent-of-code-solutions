
#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>

using namespace std;

int fuel (int mass, bool recurse = false) {
	int fuel = mass / 3 - 2; // int automatically round

	if (recurse && fuel > 0) {
		fuel += ::fuel(fuel, true);
	}

	return fuel > 0? fuel: 0;
}

int totalFuel(const vector<int> &masses, bool recurse) {
	vector <int> moduleFuels;

	for (auto mass: masses) {
		moduleFuels.push_back(fuel(mass, recurse));
	}

	int totalFuel = accumulate(moduleFuels.begin(), moduleFuels.end(), 0);

	return totalFuel;
}

vector<int> loadMasses(string filename) {
	vector<int> masses;

	ifstream file(filename);

	for (int mass; file >> mass;) {
		masses.push_back(mass);
	}

	return masses;
}


int main(int argc, char **argv) {
	bool isTest = false;
	if (argc > 1 && string(argv[1]) == "--test") {
		isTest = true;
	}
	const string filename = isTest? "data/1.ex.txt": "data/1.txt";



	const auto masses = loadMasses(filename);

	// Part 1
	cout << "answer 1 " << totalFuel(masses, false) << endl;

	// Part 2
	cout << "answer 2 " << totalFuel(masses, true) << endl;

	return 0;
}

