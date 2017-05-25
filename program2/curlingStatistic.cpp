// libraries
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "curlingStatistic.h"
using namespace std;

/********************************************************************************************************************************************
* Method: curlingStatistic
* Process: class constructor-to instantiate all member variables
* Parameter: None
**********************************************************************************************************************************************/
curlingStatistic::curlingStatistic()
{
	// instantiations of curler object, modifies member variables
	curlerName		= "";
	curlerPosition	= "";
	totalScore		= 0;
	numOfScore		= 0;

}

/********************************************************************************************************************************************
* Method: addScore
* Process: continuously add score to object total score
* Parameter: add value integer
**********************************************************************************************************************************************/
void curlingStatistic::addScore(const int addValue)
{
	// for modifying scores of curler
	totalScore += addValue;
	numOfScore += 1;
}

/********************************************************************************************************************************************
* Method: getTotalScore
* Process: return object total score
* Parameter: None
**********************************************************************************************************************************************/
int curlingStatistic::getTotalScore()const 
{
	// get total score
	return totalScore;
}

/********************************************************************************************************************************************
* Method: getScoreCount
* Process: return object total score count
* Parameter: None
**********************************************************************************************************************************************/
int curlingStatistic::getScoreCount()const 
{
	// get score count
	return numOfScore;
}

/********************************************************************************************************************************************
* Method: getCurlingPercentage
* Process: return object curling percentage
* Parameter: None
**********************************************************************************************************************************************/
double curlingStatistic::getCurlingPercentage()const 
{
	// return curling percentage
	double curlingPercentage = (totalScore / (numOfScore * max)) * 100;
	return curlingPercentage;
}

/********************************************************************************************************************************************
* Method: getPlayerPosition
* Process: return object position string
* Parameter: None
**********************************************************************************************************************************************/
string curlingStatistic::getPlayerPosition()const 
{
	// for returning curler's name
	return curlerPosition;
}

/********************************************************************************************************************************************
* Method: getPlayerName
* Process: return object name string
* Parameter: None
**********************************************************************************************************************************************/
string curlingStatistic::getPlayerName()const 
{
	// for returning curler's position
	return curlerName;
}

/********************************************************************************************************************************************
* Method: setTotalScore
* Process: set objects total score and score count
* Parameter: total score and score count integer
**********************************************************************************************************************************************/
void curlingStatistic::setTotalScore(const int totScore, const int scoreCount) 
{
	// for use with team object when setting values manually
	totalScore = totScore;
	numOfScore = scoreCount;
}

/********************************************************************************************************************************************
* Method: setNameAndPosition
* Process: set objects name and position
* Parameter: name and position string
**********************************************************************************************************************************************/
void curlingStatistic::setNameAndPosition(const string playerName, const string playerPosition) 
{
	// for modifying player name and position
	curlerName		= playerName;
	curlerPosition	= playerPosition;
}

/********************************************************************************************************************************************
* Method: plyrObjUpd
* Process: error handling for player object values
* Parameter: player object and input val
**********************************************************************************************************************************************/
void plyrObjUpd(curlingStatistic& playerObj, string inpVal, const int iter) {
	// can't have const modifier on playerObject as its data members are being modified

	// player positions
	const vector<string> positions{ "Lead", "Second", "VSkip", "Skip" };			// for assigning player position

	// for player input string
	size_t spaceOnePos{ inpVal.find(' ') };

	// get second occurence of  " " in value string for players
	size_t spaceTwoPos{ inpVal.find('\t', spaceOnePos + 1) };

	// error check string
	if (spaceOnePos == string::npos || spaceTwoPos == string::npos) {
		// tabs take presidence over spaces in use
		spaceTwoPos = inpVal.find(' ', spaceOnePos + 1);
		if (spaceTwoPos == string::npos) {
			throw "player name or score missing or in wrong order, program unable to continue \n";
		}
	}

	// for storing player's name and position
	playerObj.curlerName		= inpVal.substr(0, spaceTwoPos);
	playerObj.curlerPosition	= positions[iter - 1];

	// player score string
	const string score{ inpVal.substr(spaceTwoPos + 1, inpVal.length()) };
	bool run{ false };																// for finding first char
	size_t control{ 0 };															// for dealing with blank chars
	for (size_t i = 0; i < score.length(); i++) {
		if ((score[i] != ' ') && (score[i] != '\t')){
			run = true;																// make true if first char found
		}
		if (run == true) {
			if ((control % 2) == 0) {
				playerObj = score[i] + playerObj;									// important operation for modifying player object variables
			}
			control += 1;															// deal with ' ' spaces between each score 
		}
	}
}

/********************************************************************************************************************************************
* operator: >>
* Process: cin operator for storing character input into object
* Parameter: cin input and player object
**********************************************************************************************************************************************/
istream& operator >> (istream& input, curlingStatistic& playerObject) {	// will be using cin >> object (not a class member function)
	// can't have const modifier on playerObject as its data members are being modified
	// validate cin input character by setting try-catch block (both input and playerObject being passed by reference)
	// stringstream convert;	// if you needed to convert to type string		// almost 20 lines of code, just implementing error handling

	// string to use for operation
	string useVal;
	static int iter{ 0 };
	getline(input, useVal);															// get entire line of input
	
	if (useVal == "") {															// if value string is "", throw string exception
		throw "input can't be blank, object can't store value  \n";
	}

	// check to make sure there's at least one character letter in string
	if (!(useVal[0] > 64 && useVal[0] < 91) &&
		!(useVal[0] > 96 && useVal[0] < 123)) {
		// if value string is without char, throw string exception
		throw "first character must be letter of alphabet, object can't store value \n";
	}

	// extract values to score and player name
	const unsigned int counter = useVal.find(' ');
	if (iter > 0) {	// for when score and name included

		// function for dealing with inputs for players, not team
		plyrObjUpd(playerObject, useVal, iter);

		// increment iter
		iter += 1;																

		// reset iter in order to prevent out of range
		if (iter >= 5) {
			iter -= 5;
		}
	}
	else {
		// for when only name included (team)
		if (useVal.find(' ') != string::npos) {
			playerObject.curlerName		= useVal.substr(0, (counter));
		}
		else {
			playerObject.curlerName		= useVal.substr(0, (useVal.length()));
		}
		playerObject.curlerPosition = "Team";									// assign position
		iter						+= 1;										// increment iter
	}
	return input;
}

/********************************************************************************************************************************************
* operator: <<
* Process: cout operator for transferring object values to console
* Parameter: cout output and player object
**********************************************************************************************************************************************/
ostream& operator<< (ostream& output, const curlingStatistic& playerObject) {			// will be using cout << object

	output << playerObject.curlerName << " " << fixed << setprecision(2) <<				// for outputing to console player name and percent
			playerObject.getCurlingPercentage() << "%" << "\n";
	return output;
}

/********************************************************************************************************************************************
* operator: <
* Process: less than operator for comparing curling percentage of player objects
* Parameter: two player objects to be compared
**********************************************************************************************************************************************/
bool operator <(const curlingStatistic& playerObj, const curlingStatistic& playerObj2) {		// for comparing player's scores
	bool val = false;
	if (playerObj.getCurlingPercentage() < playerObj2.getCurlingPercentage()) {
		val = true;
	}
	return val;
}

/********************************************************************************************************************************************
* operator: >
* Process: greater than operator for comparing curling percentage of player objects
* Parameter: two player objects to be compared
**********************************************************************************************************************************************/
bool operator >(const curlingStatistic& playerObj, const curlingStatistic& playerObj2) {		// for comparing player's scores
	bool val = false;
	if (playerObj.getCurlingPercentage() > playerObj2.getCurlingPercentage()) {
		val = true;
	}
	return val;
}

/********************************************************************************************************************************************
* operator: <=
* Process: less than or equal to operator for comparing curling percentage of player objects
* Parameter: two player objects to be compared
**********************************************************************************************************************************************/
bool operator <=(const curlingStatistic& playerObj, const curlingStatistic& playerObj2) {
	bool val = false;
	if (!(playerObj > playerObj2)) {
		val = true;
	}
	return val;
}

/********************************************************************************************************************************************
* operator: >=
* Process: greater than or equal to operator for comparing curling percentage of player objects
* Parameter: two player objects to be compared
**********************************************************************************************************************************************/
bool operator >=(const curlingStatistic& playerObj, const curlingStatistic& playerObj2) {
	bool val = false;
	if (!(playerObj < playerObj2)) {
		val = true;
	}
	return val;
}

/********************************************************************************************************************************************
* operator: ==
* Process: equal to operator for comparing curling percentage of player objects
* Parameter: two player objects to be compared
**********************************************************************************************************************************************/
bool operator ==(const curlingStatistic& playerObj, const curlingStatistic& playerObj2) {
	bool val = false;
	if (!(playerObj > playerObj2) && !(playerObj < playerObj2)) {
		val = true;
	}
	return val;
}

/********************************************************************************************************************************************
* operator: !=
* Process: not equal to operator for comparing curling percentage of player objects
* Parameter: two player objects to be compared
**********************************************************************************************************************************************/
bool operator !=(const curlingStatistic& playerObj, const curlingStatistic& playerObj2) {
	bool val = false;
	if ((playerObj > playerObj2) || (playerObj < playerObj2)) {
		val = true;
	}
	return val;
}

/********************************************************************************************************************************************
* operator: +
* Process: adds character value to player objects and return new player object
* Parameter: player objects and value to be added
**********************************************************************************************************************************************/
curlingStatistic& operator+(curlingStatistic& playerObj, const char& inpValue) {			// for adding player's scores
	/*it's always a good idea in general to have this operator as you
	never know when you might need to add multiple object values of
	the same class or of different classes together and the original 
	addition operator doesn't support the operation of adding classes*/
	// this method is for obj = obj + int

	// check to see if character is character 1 to 7 or x or X
	if ((inpValue >= '0' && inpValue < '7') ||
		(inpValue == 'x') || (inpValue == 'X')) {

		if (inpValue == 'x' || inpValue == 'X' || inpValue == ' ') {		// statement to check if char is X or x
			// nothing changes if character is X or x
				
		}
		else {
			// change new obj to increment totScore by inpValue and numOfScore by 1
			const int valToAdd = (inpValue - 48);
			playerObj.totalScore += valToAdd; 
			playerObj.numOfScore += 1;
		}
		return playerObj;													// return the object
	}
	else {
		// if character isn't character 1 to 7 or x or X, throw string exception
		throw "invalid score input, object can't store value \n";
	}
}

/********************************************************************************************************************************************
* operator: +
* Process: adds character value to player objects and return new player object
* Parameter: player objects and value to be added
**********************************************************************************************************************************************/
curlingStatistic& operator+(const char& inpValue, curlingStatistic& playerObj) {				// for adding player's scores
	// this method is for obj = int + obj

	// create object to be returned
	playerObj = playerObj + inpValue;
	return (playerObj);
}
