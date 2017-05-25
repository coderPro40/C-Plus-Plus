#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include "OvernightPackage.h"
#include "TwoDayPackage.h"
#include "packages.h"
#include "client.h"
#include "fileParser.h"
#include "truck.h"
 

#ifndef UTILITY_H
#define UTILITY_H

struct Informations{
	int ID;	
	int weight;
	int priority;
	int senderID;
	int receiverID;
	int overDueLength;
	int tracker;
};

class Utility{
    public:
	    Utility(){};
	    void readClients(vector<Client*>& clients);
	    void writeClients(vector<Client*> &clients, const string fileName);
	    location* readWarehouseAddress();
	    void createPackages(const string fileName, deque<OvernightPackage*>& nightPack, deque<TwoDayPackage*>& dayPack, deque<packages*>& regular, vector<Client*>& clients);
	    void overwritePackagesToFile(const string fileName, vector<packages*>& pkgs);
	    void appendPackagesToFile(const string fileName, vector<packages*>& pkgs);
	    Informations* readInfo(const string packageInformation);
	    void updateData(vector<packages*>& selectedPackages);
		void checkID(const vector<int>& IDs, const int newID);
		void startSort(deque<OvernightPackage*>& nightPack, deque<TwoDayPackage*>& dayPack, deque<packages*>& pack);
		void loadInTruckAndDeliver(location*&, vector<Truck*>& pkgs);
		void mergePackageTypes(vector<packages*>& pkgs, deque<OvernightPackage*> overnight, deque<TwoDayPackage*> twoDay, deque<packages*> regular);
};

#endif 