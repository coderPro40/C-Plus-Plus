#pragma once

// libraries being used
#include <sstream>
#include <string>
using namespace std;

#ifndef FILEPARSER_H
#define FILEPARSER_H

// template functions
template <typename T>
void stringParse(const string& inpStr, T& inpVal) {	// function for parsing strings
	stringstream parse;								// string stream for converting from string to integer
	parse << inpStr;								// move string to stream
	parse >> inpVal;								// convert to type integer
}
#endif // !FILEPARSER_H