/*
 * 7.cpp
 *
 *  Created on: 29 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <iomanip>

using namespace std;

typedef map<char, set<char>> Targets;

#define tout if (isTest) cout

void printTargets(map<char, set<char>> targets) {
	for (const auto &target: targets) {
		cout << "target " << target.first << endl;
		for (auto dep: target.second) {
			cout << "  " << dep << endl;
		}
	}
}

Targets loadTargets(string filename) {
	Targets targets;

	ifstream file(filename);

	for (string line; getline(file, line);) {
		auto target = line[36];
		auto dependency = line[5];
		targets[target].insert(dependency);
		targets[dependency]; //Make sure that the target exists
	}
	return targets;
}


set<char> getAvailableTargets(const Targets &targets) {
	set<char> available;

	for (auto &target: targets) {
		if (target.second.empty()) {
			available.insert(target.first);
		}
	}

	return available;
}

void markReady(Targets &targets, char ready) {
	targets.erase(ready);
	for (auto &target: targets) {
		target.second.erase(ready);
	}
}

char popAvailableTarget(Targets &targets) {
	char popped;

	for (auto &target: targets) {
		if (target.second.empty()) {
			popped = target.first;
			break;
		}
	}
	markReady(targets, popped);

	return popped;
}

int main(int argc, char **argv) {
	bool isTest = false;

	if (argc > 1 && string(argv[1]) == "--test") {
		isTest = true;
	}

	const string filename = isTest? "7ex.txt": "7.txt";
	const int numWorkers = isTest? 2: 5;
	const int baseTime = isTest? 0: 60;

	{
		// Part1
		auto targets = loadTargets(filename);

		if (isTest) printTargets(targets);

		cout << "answer 1: ";
		while (!targets.empty()) {
			auto available = popAvailableTarget(targets);
			cout << available;
		}
		cout << endl;
	}

	{
		// Part 2
		auto targets = loadTargets(filename);

		auto calculateAssemblyTime = [=](char target) {
			return baseTime + target - 'A' + 1;
		};

		map<char, int> timeSpent;

		string ready;

		vector <char> currentTasks; // A task is a target worked on by an elf
		int passedTime = 0;
		for (passedTime = 0; !targets.empty(); ++passedTime) {
			tout << setw(3) << passedTime << " ";

			auto availableTargets = getAvailableTargets(targets);

			// Make sure that everybody has a task to work on
			for (auto target: availableTargets) {
				if (currentTasks.size() >= numWorkers) {
					break;
				}
				auto isNotWorked = true;
				for (auto task: currentTasks) {
					if (task == target) {
						isNotWorked = false;
					}
				}
				if (isNotWorked) {
					currentTasks.push_back(target);
				}
			}

			// Do the work
			for (auto it = currentTasks.begin(); it != currentTasks.end();) {
				auto target = *it;
				tout << target;

				auto &time = timeSpent[target];
				++time;

				if (time >= calculateAssemblyTime(target)) {
					markReady(targets, target);
					ready.push_back(target);
					it = currentTasks.erase(it);
				}
				else {
					++it;
				}
			}

			tout << "\t" << ready << "\t(";
			tout << string(availableTargets.begin(), availableTargets.end());
			tout << ")" << endl;
		}

		cout << "answer 2: it is ready after " << passedTime << " seconds" << endl;
	}

}



