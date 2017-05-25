#pragma once

#include <string>
#include <iostream>
using namespace std;

#ifndef CURLINGSTATISTIC_H
#define CURLINGSTATISTIC_H

class curlingStatistic
{
public:
	// publicily accessable functions
	explicit curlingStatistic();
	void addScore(const int addValue);
	int getTotalScore()const;
	int getScoreCount()const;
	double getCurlingPercentage()const;
	string getPlayerPosition()const;
	string getPlayerName()const;
	void setTotalScore(const int totScore, const int scoreCount);
	void setNameAndPosition(const string playerName, const string playerPosition);
	friend void plyrObjUpd(curlingStatistic& playerObj, string inpVal, const int iter);					// for adding values to player object
	friend std::istream& operator>> (istream& input, curlingStatistic& playerObject);					// will be using cin >> object
	friend std::ostream& operator<< (ostream& output, const curlingStatistic& playerObject);			// will be using cout << object
	friend bool operator <(const curlingStatistic& playerObj, const curlingStatistic& playerObj2);		// for comparing player's scores
	friend bool operator >(const curlingStatistic& playerObj, const curlingStatistic& playerObj2);		// for comparing player's scores
	friend bool operator <=(const curlingStatistic& playerObj, const curlingStatistic& playerObj2);
	friend bool operator >=(const curlingStatistic& playerObj, const curlingStatistic& playerObj2);
	friend bool operator ==(const curlingStatistic& playerObj, const curlingStatistic& playerObj2);
	friend bool operator !=(const curlingStatistic& playerObj, const curlingStatistic& playerObj2);
	friend curlingStatistic& operator+(curlingStatistic& playerObj, const char& inpValue);				// for adding player's scores
	friend curlingStatistic& operator+(const char& inpValue, curlingStatistic& playerObj);				// for adding player's scores
	curlingStatistic operator=(curlingStatistic& playerObj) {
		return(playerObj);
	}

private:
	// class member variables
	const double max{ 4 };
	string curlerName, curlerPosition;
	int totalScore, numOfScore;

protected:
	
};
#endif // ndef:
