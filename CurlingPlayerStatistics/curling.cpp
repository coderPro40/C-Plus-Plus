// curling.cpp : Defines the entry point for the console application.
//

// libraries
#include "curlingStatistic.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// function prototypes
void runOperation(vector<curlingStatistic>& player1, vector<curlingStatistic>& player2);				// prepare input and transfer output
void inputOp(vector<curlingStatistic>& player, unsigned int& totScore, unsigned int& numScore);			// for retrieving inputs sequentially
void comparisonOp(const vector<curlingStatistic>& player1, const vector<curlingStatistic>& player2);	// team stat comparison

/***********************************************************************************************************************************************
* Method: main
* Process: creates and directs process for player object inputs and outputs
* Parameter: None
* PreCondition: None
* PostCondition: None
*************************************************************************************************************************************************/
int main()
{
	try{																			// try catch block for handling errors and exceptions
		// create vector container for curling statistic objects
		vector<curlingStatistic> player1;
		vector<curlingStatistic> player2;

		// populate vector
		for (size_t i = 0; i < 5; i++) {
			// create curling statistic objects
			curlingStatistic inp, inp2;

			// store in vectors
			player1.push_back(inp);
			player2.push_back(inp2);
		}

		// for carrying out cascading of overloaded operators
		runOperation(player1, player2);
			
	}catch (const char *e){
		// catch block catch exception and output string
		cout << "\nexception occurred \n" << e;
	}
	
	return 0;
}

/***********************************************************************************************************************************************
* Method: runOperation
* Process: provide means of updating curling statistic data members
* Parameter: player1 vector and player 2 vector
* PreCondition: vector created and populated
* PostCondition: vectors curling statistic objects updated
*************************************************************************************************************************************************/
void runOperation(vector<curlingStatistic>& player1, vector<curlingStatistic>& player2) {
	// no const included as curling percentage objects need to be modified
	// provide input for each object - cascading style, was thinking of this, but couldn't account for output prompts
	/*cin >> player1[0] >> player1[1] >> player1[2] >> player1[3] >> player1[4];
	cin >> player2[0] >> player2[1] >> player2[2] >> player2[3] >> player2[4];*/

	// get total team score and team score count
	unsigned int totScore{ 0 }, totScore2{ 0 }, numScore{ 0 }, numScore2{ 0 };

	// provide information on teams
	inputOp(player1, totScore, numScore);
	inputOp(player2, totScore2, numScore2);

	// store information in team object
	player1[0].setTotalScore(totScore, numScore);
	player2[0].setTotalScore(totScore2, numScore2);

	// output each object's name and curling percentage - with cascading
	cout << "\n" << player1[0] << player1[1] << player1[2] << player1[3] << player1[4];
	cout << "\n" << player2[0] << player2[1] << player2[2] << player2[3] << player2[4];

	// run comparison operation
	comparisonOp(player1, player2);
}

/***********************************************************************************************************************************************
* Method: inputOp
* Process: update each curling statistic object using istream operator
* Parameter: player vector, totScore and numScore
* PreCondition: player vector initialized and populated, totScore and numScore initialized
* PostCondition: player vector curling statistic objects, totScore and numScore updated
*************************************************************************************************************************************************/
void inputOp(vector<curlingStatistic>& player, unsigned int& totScore, unsigned int& numScore) {
	// no const included as curling percentage objects are being modified
	// player positions
	const vector<string> positions{ "Team", "Lead", "Second", "VSkip", "Skip" };			// for assigning player position

	// loop through each object
	for (size_t i = 0; i < player.size(); i++) {
		// provide input for each object - cascading style
		(i == 0) ? (cout << "please enter " << positions[i] << " name: ") :
			(cout << "please enter " << positions[i] << " name and score: ");
		cin >> player[i];																	// store info in player obj

		// get total score from each player
		totScore += player[i].getTotalScore();

		// get score count from each player
		numScore += player[i].getScoreCount();
	}
}

/***********************************************************************************************************************************************
* Method: comparisonOp
* Process: compare curling statistic objects using relational operators
* Parameter: vector of player 1 and 2 curling statistic objects
* PreCondition: curling statistic objects' data members updated
* PostCondition: None
*************************************************************************************************************************************************/
void comparisonOp(const vector<curlingStatistic>& player1, const vector<curlingStatistic>& player2) {
	// statement preceding outputs
	cout << "\nTop statistics by position \n";

	// loop through each object of player classes
	for (size_t i = 0; i < player1.size(); i++) {
		// for when player 1 obj curling stats is greater than player 2's
		if (player1[i] > player2[i]) {
			cout << "\n" << player1[i].getPlayerPosition() << ": " << player1[i].getPlayerName();
		}

		// for when player 2 obj curling stats is greater than player 1's
		else if(player1[i] < player2[i]){
			cout << "\n" << player2[i].getPlayerPosition() << ": " << player2[i].getPlayerName();
		}

		// for when both player's curling stats are tied
		else{
			cout << "\n" << player1[i].getPlayerPosition() << ": Tie! " << player1[i].getPlayerName() <<
				", " << player2[i].getPlayerName();
		}
	}
}
