#include "packages.h"
#include "OvernightPackage.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/*******************************************************************************************************************************************************
* Method: OvernightPackage
* Process: constructor for overnight package class with inheritance to Package base class constructor
* Parameter: const unsigned int inpWeight, const double inpCost, const double inpAFPO
*********************************************************************************************************************************************************/
OvernightPackage::OvernightPackage(const int inpWeight, const double inpCost, const double inpAFPO, vector<Client*>& clnts) : packages(inpWeight, inpCost, clnts) {
	// make sure additional cost per ounce isn't 0
	if ((inpAFPO <= 0)) { throw "additional cost per ounce can't be 0, negative or blank"; }

	// update OvernightPackage object
	addFPO = inpAFPO;
}

/***********************************************************************************************************************************************************
* Method: calculateCost
* Process: calculate and return cost of shipment with addtional cost added to cost per ounce
* Parameter: None
*************************************************************************************************************************************************************/
double OvernightPackage::calculateCost()const {
	// inheritance of package calculateCost method
	const double newCPO{ addFPO + packages::cPo };	// can implement without Package:: just like weights below, but useful for distinguishing
	const double totCost{ newCPO * weight };		// get cost by adding additional cost to standard cost and mult by weight
	return totCost;
}

/***********************************************************************************************************************************************
* Method: writePackage
* Process: append package info to file
* Parameter: file
*************************************************************************************************************************************************/
void OvernightPackage::writePackage(ofstream& output) {
	output << packageID << " " << weight << " " <<				// write file info to textfile
		packPriority << " " << senderID << " " << receiverID << "\n";
}