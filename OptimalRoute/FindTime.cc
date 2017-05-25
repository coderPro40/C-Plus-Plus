
#include <algorithm>
#include "FindTime.h"
#include <bitset>
#include <string>
#include <sstream>
#include <numeric>
#include <cmath>
#include <iomanip>
using namespace std;

// function prototypes
void setBits(const string direction, bitset<8>& set);

findTime::findTime(){}

findTime::~findTime(){}

//This function returns the last house number on that avenue/street (increasing)
int findTime::FindIntervalLeft(int number) {
	int R = number % 100;
	int intervalLeft = (number + 100) - (R);
	return intervalLeft;
};
//This function returns the last house number on that avenue/street(decreasing)
int findTime::FindIntervalRight(int number) {
	int R = number % 100;
	int intervalRight = number - R;
	return intervalRight;
}
//This function returns the amount of time it takes from one location to another including stop time
int findTime::GetTime(location*& l1, location*& l2) {
	int oneStop = 5;
	int Time = oneStop;
	string TempOne = convert(GetStreetType(l1)), TempTwo = convert(GetStreetType(l2));
	size_t spaceOnePos = TempOne.find(' ', 1), spaceTwoPos = TempTwo.find(' ', 1);
	int houseNumberOne = stoi(l1->getHouseNumber()), houseNumberTwo = stoi(l2->getHouseNumber());
	const string TypeOne{ TempOne.substr(1, spaceOnePos - 1) }, TypeTwo{ TempTwo.substr(1, spaceTwoPos - 1) };
	string streetOne = l1->getAlignment(), streetTwo = l2->getAlignment();
	//If both locations are on the same street or avenue run this code
	if (streetOne == streetTwo) {
		Time = Substract(houseNumberOne, houseNumberTwo) + oneStop;
	}
	//Otherwise
	else {
		if ((TypeOne == "ST") && (TypeTwo == "AVE")) {
			Time = FromStreetToAvenue(l1, l2) + oneStop;
		}
		else if ((TypeOne == "AVE") && (TypeTwo == "ST")) {
			Time = FromAvenueToStreet(l1, l2) + oneStop;
		}
		else if ((TypeOne == "ST") && (TypeTwo == "ST")) {
			Time = FromStreetToStreet(l1, l2) + oneStop;
		}
		else if ((TypeOne == "AVE") && (TypeTwo == "AVE")) {
			Time = FromAvenueToAvenue(l1, l2) + oneStop;
		}
		//another else if for avenue to street
	}

	return Time;
};
//This function returns the distance to a particular avenue/street
int findTime::GetPointOfIntersect(string StreetType) {
	int number;
	if (isdigit(StreetType[0])) {
		number = stoi(StreetType) * 100;
	}
	//Otherwise it must be main or central avenue
	else {
		number = 0;
	}
	return number;
}

void setBits(const string direction, bitset<8>& set) {
	if (direction == "N"){
		set.set(0);
	}
	else if (direction == "S") {
		set.set(1);
	}
	else if (direction == "E") {
		set.set(2);
	}
	else if (direction == "W") {
		set.set(3);
	}
	else if (direction == "NE") {
		set.set(4);
	}
	else if (direction == "NW") {
		set.set(5);
	}
	else if (direction == "SE") {
		set.set(6);
	}
	else{
		set.set(7);
	}
}

//Substract two numbers(Making sure we never get a negative number).
int findTime::Substract(int number, int numberTwo) {
	int result;
	if (number > numberTwo) {
		result = number - numberTwo;
	}
	else {
		result = numberTwo - number;
	}
	return result;
}
//This function is as similar as the GetpointofIntersect(). Distance to avenue/street
int findTime::crossingRoad(location*& addressOne, location*& addressTwo, string directionOne, string directionTwo) {
	string TypeOne = convert(GetStreetType(addressOne));
	string TypeTwo = convert(GetStreetType(addressTwo));
	int main = 0;
	int central = 0;
	int Time{ 0 };
	bitset<8> bitflag;								// for running bits comparisons
	setBits(directionOne, bitflag);					// set bits for direction one
	setBits(directionTwo, bitflag);					// set bits for direction Two
	string direction1 = directionOne, direction2 = directionTwo;
	string AddressOne = addressOne->getAvenue(), AddressTwo = addressTwo->getAvenue();
	//
	int TypeOneNumber = GetPointOfIntersect(AddressOne), TypeTwoNumber = GetPointOfIntersect(AddressTwo);

	// all possible directions
	const string one{ "N" }, two{ "S" }, three{ "E" }, four{ "W" }, five{ "NE" }, six{ "NW" }, seven{ "SE" }, eight{ "SW" };
	
	// condition one bits
	bitset<8> con1;																			// bits for SE, SW, NE or NW 
	setBits(eight, con1); setBits(seven, con1); setBits(six, con1); setBits(five, con1);	// all bits for condition one
	con1 &= bitflag;

	// condition two bits
	bitset<8> con2;																			// bits for SE, SW, NE, NW, E or W
	setBits(eight, con2); setBits(seven, con2); setBits(three, con2); setBits(four, con2);	// all bits for condition two
	setBits(six, con2); setBits(five, con2);
	con2 &= bitflag;

	bitset<8> result;

	// we know then that both directions are towards SE, SW, NE or NW
	if ((con1.count() > 1) && (result.count() == 0)){	
		bitset<8> con1a, con1b;							// create bitset for testing that direction of NE is not going to NW and SE is not going to SW and vice versa

		// all bits for subproblems of condition one
		setBits(eight, con1a); setBits(seven, con1a); setBits(six, con1b); setBits(five, con1b);		

		// con1a for when direction 1 is equal to SW or SE, and con1b for NE or NW
		(direction1 == seven || direction1 == eight) ? result = (bitflag & con1b) : result = (bitflag & con1a);
		(result.count() > 0) ? Time = TypeOneNumber + (TypeTwoNumber) : result.reset();	// if number of 1 bits in result is greater than 0, set time based on formula, otherwise reset bits in result
	}

	// we know then that both directions are towards SE, SW, NE, NW, E or W
	if ((con2.count() > 0) && (result.count() == 0)) {
		// make sure direction one isn't E or W 
		if (direction1 == three || direction1 == four) {
			result = bitflag;
		}

		// process works if result is equal to 0 because it show NW, NE is not with direction if one directions is SE, SW
		if (result.count() == 0){
			result = con2;			// set result to con2 in order to update result count
			
			// from street to street
			if ((TypeOne == "ST") && (TypeTwo == "ST")) {
				// for when type one == street and type one == type two
				Time = Substract(TypeOneNumber, TypeTwoNumber);
			}

			// from avenue to avenue
			else {
				// for when both directions are equal
				(direction1 == direction2) ? Time = Substract(TypeOneNumber, TypeTwoNumber) : Time = (TypeOneNumber + TypeTwoNumber);
			}
		}
		else{
			// set bits back to 0 to be tested on other conditionals
			result.reset();			
		}
	}

	// last conditional if result hasn't been decided
	if (result.count() == 0) {
		bitset<8> con3a, con3b;		// create bitset for testing that dir1 of N, E, S, W is going to NE, NW, SE, SW

		// all bits for subproblems of condition three
		setBits(direction2, con3b);	// make sure direction 2 is either NE, NW, SE, SW
		setBits(eight, con3a); setBits(seven, con3a); setBits(six, con3a); setBits(five, con3a);
		result = (con3b & con3a);	// and results

		if (result.count() > 0) {
			Time = TypeTwoNumber;
		}
		result.reset();
	}
	return Time;

}

//street to avenue
int findTime::FromStreetToAvenue(location*& l1, location*& l2) {
	int Time{ 0 };
	string streetTypeOne = l1->getAvenue();
	string streetTypeTwo = l2->getAvenue();
	int houseNumberOne = stoi(l1->getHouseNumber());
	int houseNumberTwo = stoi(l2->getHouseNumber());
	string direction1 = l1->getDirection();
	string direction2 = l2->getDirection();
	//Within NW, NE, SW, SE
	//include west and east
	if ((direction1 == "NW" && direction2 == "NW") || (direction1 == "NE" && direction2 == "NE") || (direction1 == "SE" && direction2 == "SE") || (direction1 == "SW" && direction2 == "SW")) {
		Time = Substract(houseNumberOne, GetPointOfIntersect(streetTypeTwo)) + Substract(houseNumberTwo, GetPointOfIntersect(streetTypeOne));
	}
	//Horizontally(From NW to NE ) 
	if ((direction1 == "NW" && direction2 == "NE") || (direction1 == "NE" && direction2 == "NW") || (direction1 == "SE" && direction2 == "SW") || (direction1 == "SW" && direction2 == "SE")) {
		Time = Substract(houseNumberTwo, GetPointOfIntersect(streetTypeOne)) + (houseNumberOne + GetPointOfIntersect(streetTypeTwo));
	}
	// DIAGONALLY(From NW to SE) 
	if ((direction1 == "NW" && direction2 == "SE") || (direction1 == "E" && direction2 == "SW") || (direction1 == "E" && direction2 == "NW") || (direction1 == "W" && direction2 == "SE") || (direction1 == "W" && direction2 == "NE") || (direction1 == "NE" && direction2 == "SW") || (direction1 == "SE" && direction2 == "NW") || (direction1 == "SW" && direction2 == "NE")) {
		Time = houseNumberOne + GetPointOfIntersect(streetTypeTwo) + houseNumberTwo + GetPointOfIntersect(streetTypeOne);
	}
	//VERTICALLY (FROM SE to NE)
	if ((direction1 == "SE" && direction2 == "NE") || (direction1 == "E" && direction2 == "SE") || (direction1 == "E" && direction2 == "NE") || (direction1 == "W" && direction2 == "SW") || (direction1 == "W" && direction2 == "NW") || (direction1 == "NW" && direction2 == "SW") || (direction1 == "SW" && direction2 == "NW") || (direction1 == "NE" && direction2 == "SE")) {
		Time = Substract(houseNumberOne, GetPointOfIntersect(streetTypeTwo)) + (houseNumberTwo + GetPointOfIntersect(streetTypeOne));
	}
	//To central avenue North
	if ((direction1 == "SE" && direction2 == "N") || (direction1 == "SW" && direction2 == "N") || (direction1 == "NW" && direction2 == "S") || (direction1 == "SW" && direction2 == "S")) {
		Time = houseNumberOne + (houseNumberTwo + GetPointOfIntersect(streetTypeOne));
	}
	//To central avenue South
	if ((direction1 == "SE" && direction2 == "S") || (direction1 == "NE" && direction2 == "N") || (direction1 == "NW" && direction2 == "N") || (direction1 == "SW" && direction2 == "S")) {
		Time = houseNumberOne + (Substract(houseNumberTwo, GetPointOfIntersect(streetTypeOne)));
	}
	//From main to central
	if ((direction1 == "W" && direction2 == "S") || (direction1 == "W" && direction2 == "N") || (direction1 == "E" && direction2 == "N")|| (direction1 == "E" && direction2 == "S")) {
		Time = houseNumberOne + houseNumberTwo;
	}
	return Time;
};
//This function returns the distance time from avenue to street
int findTime::FromAvenueToStreet(location*& l1, location*& l2) {
	int Time{ 0 };
	string streetTypeOne = l1->getAvenue();
	string streetTypeTwo = l2->getAvenue();
	int houseNumberOne = stoi(l1->getHouseNumber());
	int houseNumberTwo = stoi(l2->getHouseNumber());
	string direction1 = l1->getDirection();
	string direction2 = l2->getDirection();
	//
	if ((direction1 == "NW" && direction2 == "NW") || (direction1 == "NE" && direction2 == "NE") || (direction1 == "SE" && direction2 == "SE") || (direction1 == "SW" && direction2 == "SW")) {
		Time = Substract(houseNumberOne, GetPointOfIntersect(streetTypeTwo)) + Substract(houseNumberTwo, GetPointOfIntersect(streetTypeOne));
	}
	//
	if ((direction1 == "NW" && direction2 == "NE") || (direction1 == "N" && direction2 == "NE") || (direction1 == "N" && direction2 == "NW") || (direction1 == "S" && direction2 == "SE") || (direction1 == "S" && direction2 == "SW") || (direction1 == "NE" && direction2 == "NW") || (direction1 == "SE" && direction2 == "SW") || (direction1 == "SW" && direction2 == "SE")) {
		Time = Substract(houseNumberOne, GetPointOfIntersect(streetTypeTwo)) + (houseNumberTwo + GetPointOfIntersect(streetTypeOne));
	}
	//
	if ((direction1 == "NW" && direction2 == "SE") || (direction1 == "N" && direction2 == "SE") || (direction1 == "N" && direction2 == "SW") || (direction1 == "S" && direction2 == "NE") || (direction1 == "S" && direction2 == "NW") || (direction1 == "NE" && direction2 == "SW") || (direction1 == "SE" && direction2 == "NW") || (direction1 == "SW" && direction2 == "NE")) {
		Time = houseNumberOne + GetPointOfIntersect(streetTypeTwo) + houseNumberTwo + GetPointOfIntersect(streetTypeOne);
	}
	if ((direction1 == "SE" && direction2 == "NE") || (direction1 == "NW" && direction2 == "SW") || (direction1 == "SW" && direction2 == "NW") || (direction1 == "NE" && direction2 == "SE")) {
		Time = (houseNumberOne + GetPointOfIntersect(streetTypeTwo)) + Substract(houseNumberTwo, GetPointOfIntersect(streetTypeOne));
	}
	//
	if ((direction1 == "SE" && direction2 == "W") || (direction1 == "NE" && direction2 == "W") || (direction1 == "NW" && direction2 == "E") || (direction1 == "SW" && direction2 == "E")) {
		Time = houseNumberOne + (houseNumberTwo + GetPointOfIntersect(streetTypeOne));
	}
	if ((direction1 == "SE" && direction2 == "E") || (direction1 == "NE" && direction2 == "E") || (direction1 == "NW" && direction2 == "W") || (direction1 == "SW" && direction2 == "W")) {
		Time = houseNumberOne + (Substract(houseNumberTwo, GetPointOfIntersect(streetTypeOne)));
	}
	if ((direction1 == "S" && direction2 == "W") || (direction1 == "S" && direction2 == "E") || (direction1 == "N" && direction2 == "W") || (direction1 == "N" && direction2 == "E")) {
		Time = houseNumberOne + houseNumberTwo;
	}
	return Time;
}

int findTime::FromStreetToStreet(location*& l1, location*& l2) {
	int Time{ 0 };
	string streetTypeOne = l1->getAvenue();
	string streetTypeTwo = l2->getAvenue();
	int houseNumberOne = stoi(l1->getHouseNumber());
	int houseNumberTwo = stoi(l2->getHouseNumber());
	string direction1 = l1->getDirection();
	string direction2 = l2->getDirection();
	//within west and East , vertical
	if ((direction1 == "W" && direction2 == "NW") || (direction1 == "W" && direction2 == "SW") || (direction1 == "NW" && direction2 == "W") || (direction1 == "NW" && direction2 == "NW") || (direction1 == "NW" && direction2 == "SW") || (direction1 == "SW" && direction2 == "W") || (direction1 == "SW" && direction2 == "SW") || (direction1 == "SW" && direction2 == "NW") || (direction1 == "E" && direction2 == "NE") || (direction1 == "E" && direction2 == "SE") || (direction1 == "NE" && direction2 == "E") || (direction1 == "NE" && direction2 == "NE") || (direction1 == "NE" && direction2 == "SE") || (direction1 == "SE" && direction2 == "E") || (direction1 == "SE" && direction2 == "SE") || (direction1 == "SE" && direction2 == "NE")) {
		int closestRouteOne = 0;
		int closestRouteTwo = 0;
		int halfOne = FindIntervalRight(houseNumberOne) + 50;
		int halfTwo = FindIntervalRight(houseNumberTwo) + 50;
		//if both houses are on the same house intervals
		if (FindIntervalLeft(houseNumberOne) == FindIntervalLeft(houseNumberTwo)) {
			//find closest route of house#1 to get to the street or intersection
			closestRouteOne = ShortestPath(houseNumberOne, houseNumberTwo);
			//find closest route of house #2 to get to the street or intersection
			closestRouteTwo = ShortestPath(houseNumberTwo, houseNumberOne);
			//Shortest path ---> which direction to go
			if (closestRouteOne <= closestRouteTwo) {
				if (houseNumberOne < halfOne) {
					Time = closestRouteOne + (houseNumberTwo - FindIntervalRight(houseNumberOne)) + crossingRoad(l1, l2, direction1, direction2);
				}
				else {
					Time = closestRouteOne + (FindIntervalLeft(houseNumberOne) - houseNumberTwo) + crossingRoad(l1, l2, direction1, direction2);
				}
			}
			else {
				if (houseNumberTwo < halfTwo) {
					Time = closestRouteTwo + (houseNumberOne - FindIntervalRight(houseNumberTwo)) + crossingRoad(l1, l2, direction1, direction2);
				}
				else {
					Time = closestRouteTwo + (FindIntervalLeft(houseNumberTwo) - houseNumberOne) + crossingRoad(l1, l2, direction1, direction2);
				}
			}
		}
		//if they are on different house intervals
		else {
			if (FindIntervalLeft(houseNumberOne) < FindIntervalLeft(houseNumberTwo)) {
				Time = (houseNumberTwo - houseNumberOne) + crossingRoad(l1, l2, direction1, direction2);
			}
			else {
				Time = (houseNumberOne - houseNumberTwo) + crossingRoad(l1, l2, direction1, direction2);
			}
		}
	}
	//From west to East horizontal
	if ((direction1 == "SW" && direction2 == "NE") || (direction1 == "NW" && direction2 == "SE") || (direction1 == "W" && direction2 == "NE") || (direction1 == "W" && direction2 == "SE") || (direction1 == "W" && direction2 == "E") || (direction1 == "SW" && direction2 == "E") || (direction1 == "NW" && direction2 == "E") || (direction1 == "NW" && direction2 == "NE") || (direction1 == "SW" && direction2 == "SE")) {
		int DistancefromHouseTwoToIntersection = houseNumberTwo + FindIntervalRight(houseNumberOne);
		int DistancefromHouseOneToIntersection = houseNumberOne - FindIntervalRight(houseNumberOne);
		Time = DistancefromHouseOneToIntersection + DistancefromHouseTwoToIntersection + crossingRoad(l1, l2, direction1, direction2);
	}
	//From East to West
	if ((direction1 == "NE" && direction2 == "SW") || (direction1 == "SE" && direction2 == "NW") || (direction1 == "NE" && direction2 == "W") || (direction1 == "SE" && direction2 == "W") || (direction1 == "E" && direction2 == "W") || (direction1 == "SE" && direction2 == "SW") || (direction1 == "NE" && direction2 == "NW") || (direction1 == "E" && direction2 == "NW") || (direction1 == "E" && direction2 == "SW")) {
		int DistancefromHouseTwoToIntersection = houseNumberTwo + FindIntervalRight(houseNumberOne);
		//Time from location one to house#2 street
		int DistancefromHouseOneToIntersection = houseNumberOne - FindIntervalRight(houseNumberOne);
		//add now where they meet plus stop time
		Time = DistancefromHouseOneToIntersection + DistancefromHouseTwoToIntersection + crossingRoad(l1, l2, direction1, direction2);
	}
	return Time;
}
int findTime::FromAvenueToAvenue(location*& l1, location*& l2) {
	int Time{ 0 };
	int houseNumberOne = stoi(l1->getHouseNumber());
	int houseNumberTwo = stoi(l2->getHouseNumber());
	string direction1 = l1->getDirection();
	string direction2 = l2->getDirection();
	if ((direction1 == "SW" && direction2 == "NE") || (direction1 == "NW" && direction2 == "SE") || (direction1 == "W" && direction2 == "NE") || (direction1 == "W" && direction2 == "SE") || (direction1 == "W" && direction2 == "E") || (direction1 == "SW" && direction2 == "E") || (direction1 == "NW" && direction2 == "E")) {
		Time = houseNumberOne + houseNumberTwo + crossingRoad(l1, l2, direction1, direction2);
	}

	//From East to West
	if ((direction1 == "NE" && direction2 == "SW") || (direction1 == "SE" && direction2 == "NW") || (direction1 == "NE" && direction2 == "W") || (direction1 == "SE" && direction2 == "W") || (direction1 == "E" && direction2 == "W") || (direction1 == "E" && direction2 == "NW") || (direction1 == "E" && direction2 == "SW")) {
		Time = houseNumberOne + houseNumberTwo + crossingRoad(l1, l2, direction1, direction2);
	}
	//From south or North to anywhere only works when have same intervals
	if ((direction1 == "S" && direction2 == "SE") || (direction1 == "S" && direction2 == "SW") || (direction1 == "S" && direction2 == "NW") || (direction1 == "N" && direction2 == "NE") || (direction1 == "N" && direction2 == "SE") || (direction1 == "N" && direction2 == "SW") || (direction1 == "N" && direction2 == "NW") || (direction1 == "N" && direction2 == "NE")) {
		int closestRouteOne = 0;
		int closestRouteTwo = 0;
		int halfOne = FindIntervalRight(houseNumberOne) + 50;
		int halfTwo = FindIntervalRight(houseNumberTwo) + 50;
		//if both houses are between the same intervals
		if (FindIntervalLeft(houseNumberOne) == FindIntervalLeft(houseNumberTwo)) {
			//find closest route of house#1 to get to the street or intersection
			closestRouteOne = ShortestPath(houseNumberOne, houseNumberTwo);
			//find closest route of house #2 to get to the street or intersection
			closestRouteTwo = ShortestPath(houseNumberTwo, houseNumberOne);
			//Shortest path
			if (closestRouteOne <= closestRouteTwo) {
				if (houseNumberOne < halfOne) {
					Time = closestRouteOne + (houseNumberTwo - FindIntervalRight(houseNumberOne)) + crossingRoad(l1, l2, direction1, direction2);
				}
				else {
					Time = closestRouteOne + (FindIntervalLeft(houseNumberOne) - houseNumberTwo) + crossingRoad(l1, l2, direction1, direction2);
				}
			}
			else {
				if (houseNumberTwo < halfTwo) {
					Time = closestRouteTwo + (houseNumberOne - FindIntervalRight(houseNumberTwo)) + crossingRoad(l1, l2, direction1, direction2);
				}
				else {
					Time = closestRouteTwo + (FindIntervalLeft(houseNumberTwo) - houseNumberOne) + crossingRoad(l1, l2, direction1, direction2);
				}
			}
		}
		//if they are on different house intervals
		else {
			Time = (houseNumberTwo + houseNumberOne) + crossingRoad(l1, l2, direction1, direction2);
		}
	}
	//
	if ((direction1 == "NE" && direction2 == "NW") || (direction1 == "SW" && direction2 == "SE") || (direction1 == "SE" && direction2 == "SW") || (direction1 == "NW" && direction2 == "NE") || (direction1 == "SW" && direction2 == "SW") || (direction1 == "SE" && direction2 == "SE") || (direction1 == "NW" && direction2 == "NW") || (direction1 == "NE" && direction2 == "NE")) {
		int closestRouteOne = 0;
		int closestRouteTwo = 0;
		int halfOne = FindIntervalRight(houseNumberOne) + 50;
		int halfTwo = FindIntervalRight(houseNumberTwo) + 50;
		//if both houses are between the same intervals
		if (FindIntervalLeft(houseNumberOne) == FindIntervalLeft(houseNumberTwo)) {
			//find closest route of house#1 to get to the street or intersection
			closestRouteOne = ShortestPath(houseNumberOne, houseNumberTwo);
			//find closest route of house #2 to get to the street or intersection
			closestRouteTwo = ShortestPath(houseNumberTwo, houseNumberOne);
			//Shortest path
			if (closestRouteOne <= closestRouteTwo) {
				if (houseNumberOne < halfOne) {
					//Looks like only works when both are between the same interval for optimal result <----
					Time = closestRouteOne + (houseNumberTwo - FindIntervalRight(houseNumberOne)) + crossingRoad(l1, l2, direction1, direction2);
				}
				else {
					Time = closestRouteOne + (FindIntervalLeft(houseNumberOne) - houseNumberTwo) + crossingRoad(l1, l2, direction1, direction2);
				}
			}
			else {
				if (houseNumberTwo < halfTwo) {
					Time = closestRouteTwo + (houseNumberOne - FindIntervalRight(houseNumberTwo)) + crossingRoad(l1, l2, direction1, direction2);
				}
				else {
					Time = closestRouteTwo + (FindIntervalLeft(houseNumberTwo) - houseNumberOne) + crossingRoad(l1, l2, direction1, direction2);
				}
			}
		}
		//if they are on different house intervals
		else {
			if (FindIntervalLeft(houseNumberOne) < FindIntervalLeft(houseNumberTwo)) {
				Time = (houseNumberTwo - houseNumberOne) + crossingRoad(l1, l2, direction1, direction2);
			}
			else {
				Time = (houseNumberOne - houseNumberTwo) + crossingRoad(l1, l2, direction1, direction2);
			}
		}
	}
	return Time;

}
//Convert from lower case to upper case
string findTime::convert(string avenue) {
	transform(avenue.begin(), avenue.end(), avenue.begin(), ::toupper);
	return avenue;
}
int findTime::ShortestPath(int houseNumberOne, int houseNumberTwo) {
	int closestRouteOne = 0;
	int closestRouteTwo = 0;
	int halfOne = FindIntervalRight(houseNumberOne) + 50;
	int halfTwo = FindIntervalRight(houseNumberTwo) + 50;
	//find closest route of house#1 to get to the street or intersection
	if (houseNumberOne >= halfOne) {
		closestRouteOne = FindIntervalLeft(houseNumberOne) - houseNumberOne;
	}
	else {
		closestRouteOne = houseNumberOne - FindIntervalRight(houseNumberOne);
	}
	return closestRouteOne;
}
string findTime::GetStreetType(location*& addressOne) {
	string AddressOne = addressOne->getAvenue();
	size_t pos1 = AddressOne.find(" ");
	size_t pos2 = AddressOne.find(" ", pos1 + 1);
	size_t pos3 = 1 + AddressOne.find(" ", pos2 + 1);
	string TypeAdressOne = convert(AddressOne.substr(pos1, pos2));
	if ((TypeAdressOne == " STRE") || (TypeAdressOne == " STREE")) {
		TypeAdressOne = " STREET";
	}
	else if ((TypeAdressOne == " AVEN") || (TypeAdressOne == " AVENU")) {
		TypeAdressOne = " AVENUE";
	}
	return TypeAdressOne;
}
