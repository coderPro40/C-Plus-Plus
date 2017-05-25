#pragma once
#include <fstream>
#include "client.h"

// necessary libraries
#include <string>
using namespace std;

#ifndef Package_H
#define Package_H
// base class
class packages
{
public:
	packages(const int inpWeight, const double inpCost, vector<Client*>& clnts);	// constructor for Package base class
	double calculateCost()const;							// for calculating cost

	// newly added functions
	// void setClientsID(const int sendID, const int receiveID);	// store id for sender and receiver of package
	void setIDS(const int sendID, const int receiveID, const int packID);	// store id for sender and receiver of package
	void setPriority(const int priority);									// store priority of package
	int getPriority() const;												// get priority of package
	int getWeight() const;										// get weight of package
	int getPackageID() const;									// get id of package
	int getSenderID() const;									// get id of sender of package
	int getReceiverID() const;									// get id of receiver of package
	Client* getSender();
	Client* getReceiver();
	void writePackage(ofstream& output);									// write package info to file

protected:
	// data members weight and cost per ounce for base class 
	int weight;
	double cPo;
	int packPriority;

	// newly added data members
	int  packageID, senderID, receiverID;
	string status;
	vector<Client*> clients;
	Client* sender;
	Client* receiver;

};
#endif // !Package
