#include "packages.h"
#include "TwoDayPackage.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/***********************************************************************************************************************************************
* Method: TwoDayPackage
* Process: constructor for TwoDayPackage with inheritance to packages base class constructor
* Parameter: const unsigned int inpWeight, const double inpCost
*************************************************************************************************************************************************/
TwoDayPackage::TwoDayPackage(const int inpWeight, const double inpCost, const double inpFF, vector<Client*>& clnts): packages(inpWeight, inpCost, clnts) {
	// make sure cost per ounce and weight aren't 0
	if ((inpFF <= 0)) { throw "flat fee can't be 0, negative or blank"; }

	// update TwoDayPackage object
	flatFee = inpFF;
}

/***********************************************************************************************************************************************
* Method: calculateCost
* Process: calculate and return cost of shipment plus flat fee
* Parameter: None
*************************************************************************************************************************************************/
double TwoDayPackage::calculateCost()const {
	// inheritance of package calculateCost method
	const double totCost{ flatFee + packages::calculateCost() };		// get cost by multiplying weights and cost per ounce and adding flat fee
	return totCost;
}

/***********************************************************************************************************************************************
* Method: writePackage
* Process: append package info to file
* Parameter: file
*************************************************************************************************************************************************/
void TwoDayPackage::writePackage(ofstream& output) {
	output << packageID << " " << weight << " " <<				// write file info to textfile
		packPriority << " " << senderID << " " << receiverID << "\n";
}