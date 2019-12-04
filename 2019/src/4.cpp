/*
 * 4.cpp
 *
 *  Created on: 3 dec. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cassert>

using namespace std;

struct Password: public array<char, 6> {
	Password() {}
	Password(const char * password) {
		for (int i = 0; i < size(); ++i) {
			at(i) = password[i];
		}
	}
	Password(const string password) {
		for (int i = 0; i < size(); ++i) {
			at(i) = password[i];
		}
	}

	bool isValid() {
		char previous = front();
		bool hasDuplicate = false;
		for (int i = 1; i < size(); ++i) {
			auto c = (*this)[i];
			if (c < previous) {
				return false;
			}
			else if (c == previous) {
				hasDuplicate = true;
			}
			previous = c;
		}
		return hasDuplicate;
	}

	string str() {
		return string(begin(), end());
	}
};

vector <pair<Password, bool>> tests {
	{"111123", true},
	{"111111", true},
	{"223450", false},
	{"123789", false},
};


int main(int argc, char **argv) {
	// Test validation function
	for (auto test: tests) {
		assert(test.first.isValid() == test.second);
	}

	const int loInt = 172851;
	const int hiInt = 675869;

	int matches = 0;

	for (int i = loInt; i <= hiInt; ++i) {
		Password password(to_string(i));
		if (password.isValid()) {
			++matches;
			cout << "found valid password:" << password.str() << endl;
		}

		if (i % 100000 == 0) {
			cout << "progress " << 100L * (i - loInt) / (hiInt - loInt) << endl;
		}
	}

	cout << "answer 1: number of matches: " << matches;

}





