/*
 * 9.cpp
 *
 *  Created on: 29 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <list>

using namespace std;


struct Board {
	Board() {
		marbles.insert(marbles.begin(), 0);
	}

	vector<int> putMarble() {
		++lastMarbleNumber;

		if (lastMarbleNumber % 23 == 0) {
			vector<int> ret;
			ret.push_back(lastMarbleNumber);
			(*this) -= 7;
			ret.push_back(*currentMarble);
			currentMarble = marbles.erase(currentMarble);
			return ret;
		}
		else {
			(*this) += 1;
			marbles.insert(next(currentMarble), lastMarbleNumber);
			++currentMarble;
			++round;
			return {};
		}
	}

	void operator += (int value) {
		for (int i = 0; i < value; ++i) {
			++currentMarble;
			if (currentMarble == marbles.end()) {
				currentMarble = marbles.begin();
			}
		}
	}

	void operator -= (int value) {
		for (int i = 0; i < value; ++i) {
			if (currentMarble == marbles.begin()) {
				currentMarble = marbles.end();
			}
			--currentMarble;
		}
	}

	void print() {
		for (auto it = marbles.begin(); it != marbles.end(); ++it) {
			auto marble = *it;
			if (it == currentMarble) {
				cout << "(" << marble << ")";
			}
			else {
				cout << " " << marble << " ";
			}
		}
		cout << endl;
	}


	int play(int numPlayers, int lastMarble) {
		int currentPlayer = 1;

		vector<long> scores(numPlayers + 1); // index 0 is unused

		for (int i = 0; i <= lastMarble; ++i) {
			auto yield = putMarble();

	//		cout << "[" << currentPlayer << "] ";

			if (!yield.empty()) {
				long playerPoints = accumulate(yield.begin(), yield.end(), 0);
				scores[currentPlayer] += playerPoints;
			}

	//		board.print();

			if (++currentPlayer > numPlayers) {
				currentPlayer = 1;
			}

			if (i % 1000000 == 0 && i) {
				cout << "played marble " << i << " of " << lastMarble;
				cout << " = " << 100 * i / lastMarble << "%" << endl;
			}
		}

		auto element = max_element(scores.begin(), scores.end());

		cout << "player " << distance(scores.begin(), element) << " won with " << *element << endl;
		return *element;
	}

//	typedef vector<int> Container;
	typedef list<int> Container;

	Container marbles;
	int lastMarbleNumber = 0;
	Container::iterator currentMarble = marbles.end();
	int round = 0;
};

int main(int argc, char **argv) {
	vector<pair<int, int>> examples = {
		{9,   25},
		{10, 1618},
		{13, 7999},
		{17, 1104},
		{21, 6111},
		{30, 5807},
	};

	vector<int> exampleAnswers = {
			32,
			8317,
			146373,
			2764,
			54718,
			37305,
	};

	{
		// Examples
		for (int i = 0; i < examples.size(); ++i) {
			auto example = examples.at(i);
			auto answer = exampleAnswers.at(i);
			Board board;
			if (board.play(example.first, example.second) != answer) {
				cout << " === wrong answer === " << endl;
			}
		}
	}

	int numPlayers = 462;
	int lastMarble = 71938;
	{
		// Part 1
		Board board;

		cout << "answer 1: " << endl;
		board.play(numPlayers, lastMarble);
	}
	{
		// Part 2
		Board board;

		cout << "answer 2: " << endl;
		board.play(numPlayers, lastMarble * 100);

	}
}



