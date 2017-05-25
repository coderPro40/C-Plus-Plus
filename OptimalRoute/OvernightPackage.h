#pragma once

// necessary libraries
#include <string>
#include "packages.h"
#include <fstream>
using namespace std;

#ifndef OVERNIGHTPACKAGE_H
#define OVERNIGHTPACKAGE_H

// overnight package class inherits from package base class
class OvernightPackage : public packages
{
public:
	OvernightPackage(const int inpWeight, const double inpCost, const double inpAFPO, vector<Client*>& clnts);	// constructor for overnight shipping class
	double calculateCost()const;																// modified for calculating cost with additional fee added to cost per ounce
	void writePackage(ofstream& output);												// write package info to file

private:
	// additional class member of addition fee per ounce
	double addFPO;
};
#endif