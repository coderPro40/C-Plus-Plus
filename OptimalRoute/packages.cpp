#include "packages.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/***********************************************************************************************************************************************
* Method: Package
* Process: constructor for Package base class
* Parameter: const unsigned int inpWeight, const double inpCost
*************************************************************************************************************************************************/
packages::packages(const int inpWeight, const double inpCost, vector<Client*>& clnts) {
	// make sure cost per ounce and weight aren't 0
	if ((inpWeight <= 0) || (inpCost <= 0)) { throw "neither weight nor cost per ounce can be 0, negative nor blank"; }

	// update Package object
	weight	= inpWeight;		
	cPo		= inpCost;
	clients = clnts;			// vector of client objects pass by reference
}

Client* packages::getSender(){
	return sender;
}

Client* packages::getReceiver(){
	return receiver;
}

/***********************************************************************************************************************************************
* Method: calculateCost
* Process: calculate and return cost of shipment
* Parameter: None
*************************************************************************************************************************************************/
double packages::calculateCost()const {
	const double totCost{ weight * cPo };		// get cost by multiplying weights and cost per ounce
	return totCost;
}

/***********************************************************************************************************************************************
* Method: setClientsID
* Process: set ID of both sender and receiver of Package
* Parameter: sendID, recieveID
*************************************************************************************************************************************************/
void packages::setIDS(const int sendID, const int receiveID, const int packID) {
	senderID	= sendID;		// update data members
	receiverID	= receiveID;	// update data members
	packageID	= packID;		// update data members
	for (int i = 0; i < clients.size(); i++)
	{
		if(sendID == clients[i]->getId()){
			sender = clients[i];				// set sender as client object
		}
		if(receiveID == clients[i]->getId()){
			receiver = clients[i];				// also set receiver as client object
		}
	}
}

/***********************************************************************************************************************************************
* Method: setPriority
* Process: set priority of package
* Parameter: priority
*************************************************************************************************************************************************/
void packages::setPriority(const int priority) {
	packPriority = priority;	// update priority
}

/***********************************************************************************************************************************************
* Method: getPriority
* Process: get priority of package
* Parameter: None
*************************************************************************************************************************************************/
int packages::getPriority() const {
	return packPriority;		// return priority
}

/***********************************************************************************************************************************************
* Method: getWeight
* Process: get weight of Package
* Parameter: None
*************************************************************************************************************************************************/
int packages::getWeight() const {
	return weight;
}

/***********************************************************************************************************************************************
* Method: getPackageID
* Process: get ID of Package
* Parameter: None
*************************************************************************************************************************************************/
int packages::getPackageID() const {
	return packageID;
}

/***********************************************************************************************************************************************
* Method: getSenderID
* Process: get ID of sender of Package
* Parameter: None
*************************************************************************************************************************************************/
int packages::getSenderID() const{
	return senderID;
}

/***********************************************************************************************************************************************
* Method: getReceiverID
* Process: get ID of receiver of Package
* Parameter: None
*************************************************************************************************************************************************/
int packages::getReceiverID() const{
	return receiverID;
}

/***********************************************************************************************************************************************
* Method: writePackage
* Process: append package info to file
* Parameter: file
*************************************************************************************************************************************************/
void packages::writePackage(ofstream& output) {
	output << packageID << " " << weight << " " <<				// write file info to textfile
		packPriority << " " << senderID << " " << receiverID << "\n";
}