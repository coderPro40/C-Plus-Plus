#ifndef Location_H
#define Location_H
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

class location {
public:
	location(string);
	~location();

	string getHouseNumber();
	string getAvenue();
	string getDirection();
	string getAlignment();
	string getFullAddress();
protected:
	string houseNumber;
	string avenue;
	string direction;
	string Address;
	string alignment;
};

#endif