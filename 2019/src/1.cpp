
#include <iostream>
#include <vector>
#include <fstream>
#include <numeric>

using namespace std;

int fuel (int mass) {
	int fuel = mass / 3 - 2; // int automatically round

	return fuel;
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

	vector <int> moduleFuels;

	for (auto mass: masses) {
		moduleFuels.push_back(fuel(mass));
	}

	int totalFuel = accumulate(moduleFuels.begin(), moduleFuels.end(), 0);

	cout << "answer 1 " << totalFuel << endl;

	return 0;
}

