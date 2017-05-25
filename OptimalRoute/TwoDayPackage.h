#pragma once

// necessary libraries
#include <string>
#include <fstream>
#include "packages.h"
using namespace std;

#ifndef TWODAYPACKAGE_H
#define TWODAYPACKAGE_H

// two day package class inherits from package base class
class TwoDayPackage : public packages
{
public:
	TwoDayPackage(const int inpWeight, const double inpCost, const double inpFF, vector<Client*>& clnts);		// constructor for two day shipping class
	double calculateCost()const;																// modified for calculating cost plus flat fee
	void writePackage(ofstream& output);												// write package info to file

private:
	// additional class member of flat fee
	double flatFee;
};
#endif