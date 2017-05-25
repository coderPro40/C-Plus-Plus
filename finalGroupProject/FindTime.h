#pragma once
// #include "stdafx.h"
#ifndef FindTime_H
#define FindTime_H
#include <iostream>
#include <ostream>
#include <string>
#include "location.h"
using namespace std;

class findTime {
public:
	findTime();
	~findTime();
	int FindIntervalLeft(int);
	int FindIntervalRight(int);
	string GetStreetType(location*&);
	int FromStreetToAvenue(location*&, location*&);
	int FromStreetToStreet(location*&, location*&);
	int FromAvenueToAvenue(location*&, location*&);
	int FromAvenueToStreet(location*&, location*&);
	int Substract(int, int);
	int GetPointOfIntersect(string);
	int GetTime(location*&, location*&);
	// Changed crossingRoad for testing: int crossingRoad(location*&, location*&, string, string);
	int crossingRoad(location*&, location*&, string, string);
	int ShortestPath(int, int);
	string convert(string);


};

#endif