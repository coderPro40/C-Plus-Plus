#pragma once

// necessary libraries
#include <string>
#include <fstream>
#include "packages.h"
using namespace std;

#ifndef TRUCK_H
#define TRUCK_H

struct Truck {
	size_t ID{ 0 };								// truck id for identification
	size_t weight{ 0 };							// current weight on truck
	size_t minutes{ 0 };						// current minutes driven by truck
	string direction;					// direction of truck
	vector<packages*> storedPkgs;		// list of directional packages
	vector<packages*> deliveredPkges;	// list of packages delivered in run
};
#endif // !TRUCK_H