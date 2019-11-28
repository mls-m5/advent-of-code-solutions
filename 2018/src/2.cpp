/*
 * 2.cpp
 *
 *  Created on: 26 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <numeric>
#include <algorithm>
#include <iterator>


using namespace std;


vector <string> testStrings = {
		"abcdef", // no
		"bababc", // two and three
		"abbcde", // two
		"abcccd", // three
		"aabcdd", // two (count once)
		"abcdee", // two
		"ababab", // tree (count once)
};

vector <string> testStrings2 = {
		"abcde",
		"fghij",
		"klmno",
		"pqrst",
		"fguij",
		"axcye",
		"wvxyz",
};


struct CountResult {
	bool two = false;
	bool three = false;
};

CountResult count(string str) {
	map <char, int> counts;

	for (auto c: str) {
		++counts[c];
	}

	CountResult res;

	for (auto c: counts) {
		if (c.second == 2) {
			res.two = true;
		}
		if (c.second == 3) {
			res.three = true;
		}
	}
	return res;
}



int main(int argc, char **argv) {
	ifstream file("2.txt");
	vector <string> inputLines(
			(istream_iterator<string>(file)),
			istream_iterator<string>());

	std::vector<CountResult> partialResult;

	for (auto s: inputLines) {
		partialResult.push_back(count(s));
	}

//	//std::transform alternative
//	transform(
//			inputLines.begin(),
//			inputLines.end(),
//			back_inserter(partialResult),
//			[](string in) {
//		return count(in);
//	});


	int two = 0, three = 0;
	for (auto p: partialResult) {
		two += p.two;
		three += p.three;
	}

//	// Alternative (unreadable) accumulate version
//	auto res = accumulate(
//			partialResult.begin(),
//			partialResult.end(),
//			pair<int, int>{},
//			[](pair<int, int> sum, CountResult r) {
//				sum.first += r.two;
//				sum.second += r.three;
//			});
//	int two = res.first;
//	int three = res.second;

	cout << "twos " << two << endl;
	cout << "threes " << three << endl;

	cout << "product " << two * three << endl;



	cout << "Part 2" << endl;

	for (int i = 0; i < inputLines.size(); ++i) {
		for (int i2 = i; i2 < inputLines.size(); ++i2) {
			auto str1 = inputLines[i];
			auto str2 = inputLines[i2];

			int diff = 0;

			for (int j = 0; j < str1.size(); ++j) {
				if (str1[j] != str2[j]) {
					++diff;
				}
			}

			if (diff == 1) {
				string resString;

				for (int j = 0; j < str1.size(); ++j) {
					if (str1[j] == str2[j]) {
						resString.push_back(str1[j]);
					}
				}

				cout << "the answer: " << resString << endl;
				break;
			}
		}
	}
}

