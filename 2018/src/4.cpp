/*
 * 4.cpp
 *
 *  Created on: 28 nov. 2019
 *      Author: Mattias Larsson Sköld
 */

//#define isTest

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <iterator>
#include <ctime>
#include <locale>
#include <map>


const std::string testInput(R"_(
[1518-11-01 00:00] Guard #10 begins shift
[1518-11-01 00:05] falls asleep
[1518-11-01 00:30] falls asleep
[1518-11-01 00:55] wakes up
[1518-11-01 23:58] Guard #99 begins shift
[1518-11-02 00:40] falls asleep
[1518-11-02 00:50] wakes up
[1518-11-03 00:05] Guard #10 begins shift
[1518-11-03 00:24] falls asleep
[1518-11-03 00:29] wakes up
[1518-11-04 00:02] Guard #99 begins shift
[1518-11-04 00:36] falls asleep
[1518-11-04 00:46] wakes up
[1518-11-05 00:03] Guard #99 begins shift
[1518-11-05 00:45] falls asleep
[1518-11-05 00:55] wakes up
[1518-11-01 00:25] wakes up
)_");

using namespace std;

struct Entry {
	enum Action {
		ShiftStart,
		Sleep,
		Awake,
	};

	int yy = 0, mm = 0, dd = 0;
	int h = 0, m = 0;
	int guard = -1;

	Action action = ShiftStart;


	const int *begin() const { return &yy; };
	const int *end() const { return &m + 1; };

	bool operator < (const Entry& other) {
		const auto len = distance(begin(), end());
		for (size_t i = 0; i < len; ++i) {
			auto int1 = *(this->begin() + i);
			auto int2 = *(other.begin() + i);
			if (int1 < int2) {
				return true;
			}
			else if (int1 > int2) {
				return false;
			}
		}
		return false;
	}

	Entry(string line) {
		regex r("[0-9]+");
		transform(
				sregex_iterator(line.begin(), line.end(), r),
				sregex_iterator(),
				&yy, // Using pointers as iterators
				[] (smatch match) {
			return stoi(match.str());
		});

		if (h > 0) {
			h -= 24;
			dd += 1;
		}

		auto description = line.substr(19);
		if (description == "falls asleep") {
			action = Sleep;
		}
		else if (description == "wakes up") {
			action = Awake;
		}
	}

	void print() {
		// Notice that printf is used here only because it is easier for
		// this large number of arguments
		printf("%d-%02d-%02d %02d:%02d\t- \t#%d %d\n",
				yy, mm, dd,
				h, m,
				guard, action);
	}
};

int main(int argc, char **argv) {
#ifdef isTest
	istringstream input(testInput);
#else
	ifstream input("4.txt");
#endif



	vector<Entry> entries;
	for (string line; getline(input, line);) {
		if (line.empty()) {
			continue;
		}
		entries.emplace_back(line);

		entries.back().print();
	}

	sort(entries.begin(), entries.end(), [] (Entry e1, Entry e2) {
//		return e1.date < e2.date;
		return e1 < e2;
	});

	cout << "--" << endl;

	int lastGuard;
	map<int, int> sleepTime;
	for (auto &entry: entries) {
		if (entry.guard == -1) {
			entry.guard = lastGuard;
		}
		else {
			lastGuard = entry.guard;
		}

		if (entry.action == entry.Awake) {
			sleepTime[lastGuard] += entry.m;
		}
		else if (entry.action == entry.Sleep) {
			sleepTime[lastGuard] -= entry.m;
		}
		entry.print();
	}

	int sleepiestGuard = -1;
	int longestSleep = 0;
	for (auto s: sleepTime) {
		if (s.second > longestSleep) {
			longestSleep = s.second;
			sleepiestGuard = s.first;
		}
	}

	cout << "#" << sleepiestGuard << " slept most with " << longestSleep << "m" << endl;

	vector <int> sleptMinutes(60);

	for (auto &entry: entries) {
		if (entry.guard == sleepiestGuard) {
			if (entry.action == entry.Sleep) {
				for (int i = entry.m; i < 60; ++i) {
					++sleptMinutes[i];
				}
			}
			else if (entry.action == entry.Awake) {
				for (int i = entry.m; i < 60; ++i) {
					--sleptMinutes[i];
				}
			}
		}
	}

	auto mostSleptMinute = 0;
	auto longestSleptTimeOnMinute = 0;
	for (size_t i = 0; i < sleptMinutes.size(); ++i) {
		auto slept = sleptMinutes[i];
		if (slept > longestSleptTimeOnMinute) {
			longestSleptTimeOnMinute = slept;
			mostSleptMinute = i;
		}
	}

	cout << "the guard sleeps most at 00:" << mostSleptMinute << endl;


	cout << "answer 1: " << sleepiestGuard * mostSleptMinute << endl;
	cout << endl;


	// Part 2
	// Index guard, minute
	map<pair<int, int>, int> sleptMap;
	for (auto &entry: entries) {
		if (entry.action == entry.Sleep) {
			for (int i = entry.m; i < 60; ++i) {
				++sleptMap[{entry.guard, i}];
			}
		}
		else if (entry.action == entry.Awake) {
			for (int i = entry.m; i < 60; ++i) {
				--sleptMap[{entry.guard, i}];
			}
		}
	}

	int sleptMapMax = 0;
	int sleptMapGuard = -1;
	int sleptMapMinute = -1;
	for (auto s: sleptMap) {
		if (s.second > sleptMapMax) {
			sleptMapMax = s.second;
			sleptMapGuard = s.first.first;
			sleptMapMinute = s.first.second;
		}
	}

	cout << "#" << sleptMapGuard << " slept " << sleptMapMax;
	cout << " during minute " << sleptMapMinute << endl;
	cout << "answer 2: " << sleptMapGuard * sleptMapMinute;
}




