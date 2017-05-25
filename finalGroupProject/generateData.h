#pragma once
#include <iostream>
#include "client.h"
#include "location.h"
#include "utility.h"
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

#ifndef GENERATEDATA_H
#define GENERATEDATA_H
class GenerateData{
public:
	GenerateData(){};
	void generateRandClients(Utility*& util, const size_t number);	// use to generate and append random clients to client's textfile
};
#endif
