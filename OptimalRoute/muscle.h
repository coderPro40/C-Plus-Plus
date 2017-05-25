#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include "utility.h"
#include "packages.h"
#include "OvernightPackage.h"
#include "TwoDayPackage.h"
#include "truck.h"
 

#ifndef MUSCLE_H
#define MUSCLE_H

class Muscle{
    public:
	    Muscle(){};
		void deliveryTrucks(location*& warehouseLocation, vector<packages*>& pkgs, vector<packages*>& selected, vector<packages*>& pending, vector<Truck*>& trucks);			// create delivery trucks for transporting packages to different parts of the city
		/*void selectPackages(location*& warehouseLocation, deque<OvernightPackage*>& overnight, deque<TwoDayPackage*>& twoDay, deque<packages*>& regular, vector<packages*>& selected);*/
};

#endif