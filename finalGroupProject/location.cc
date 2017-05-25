#include "location.h"
#include <iostream>
#include <ostream>
#include <string>
#include <numeric>
#include <cmath>
#include <iomanip>
using namespace std;

location::location(string address)
{
	houseNumber = getHouseNumber();
	avenue = getAvenue();
	direction = getDirection();
	Address = address;
}

location::~location()
{
}
string location::getHouseNumber() {
	size_t pos1 = Address.find(" ");
	houseNumber = Address.substr(0, pos1);
	return houseNumber;
}

string location::getAvenue()
{
	size_t pos1 = Address.find(" ");
	size_t pos2 = 1 + Address.find(" ", pos1 + 1);
	avenue = Address.substr(pos1 + 1, pos2+2);
	return avenue;
}

string location::getDirection()
{
	size_t pos1 = Address.find(" ");
	size_t pos2 = 1 + Address.find(" ", pos1 + 1);
	size_t pos3 = 1 + Address.find(" ", pos2 + 1);
	direction = Address.substr(pos3);
	return direction;
}

string location::getAlignment()
{
	size_t pos1 = Address.find(" ");
	size_t pos2 = Address.find(" ", pos1 + 1);
	alignment= Address.substr(pos1 + 1);
	return alignment;
}

string location::getFullAddress(){
	return Address;
}