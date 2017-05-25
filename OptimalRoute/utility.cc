#include <fstream> 
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <typeinfo>
#include "client.h"
#include "packages.h"
#include "utility.h"
#include <fstream>
using namespace std;

void split(string line, vector<string> &words)
{
	stringstream ss(line);
	string word = "";
	while(ss >> word)
	{
		words.push_back(word);
	}
}

int parseInt(string number)
{
	stringstream ss(number);
	int intNumber;
	ss >> intNumber;
	return intNumber;
}


/*******************************************************************************************************************************************************
* Process: merges deques of all type of packages into one <packages> type vector, pkgs
*********************************************************************************************************************************************************/
void Utility::mergePackageTypes(vector<packages*>& pkgs, deque<OvernightPackage*> overnight, deque<TwoDayPackage*> twoDay, deque<packages*> regular){
	for (int i = 0; i < overnight.size(); i++)
	{
		pkgs.push_back(overnight[i]);
	}
	for (int i = 0; i < twoDay.size(); i++)
	{
		pkgs.push_back(twoDay[i]);
	}
	for (int i = 0; i < regular.size(); i++)
	{
		pkgs.push_back(regular[i]);
	}
}

/*******************************************************************************************************************************************************
* Process: loops through each package and prints the order then again loops through each packages and prints the reciever's address
* Parameter: warehouseLocation and pakcages
* PostCondition: thermial output
*********************************************************************************************************************************************************/
void Utility::loadInTruckAndDeliver(location*& warehouseLocation, vector<Truck*>& pkgs){
	//print how the muscle should load packages
	for (size_t j = 0; j < pkgs.size(); j++){
		cout << "\n\nOrder to put packages in truck " << (pkgs[j]->ID + 1) << " headed " << pkgs[j]->direction << ": " << endl;
		cout << "-----------------------------------------------" << endl;
		for (size_t i = 0; i < pkgs[j]->deliveredPkges.size(); i++){				// for each package delivered in pkgs
			// print out package ID, receiver's first name, receiver's last name and ID
			cout << pkgs[j]->deliveredPkges[i]->getPackageID() << ": " <<
			pkgs[j]->deliveredPkges[i]->getReceiver()->getFirstName() <<
			" " << pkgs[j]->deliveredPkges[i]->getReceiver()->getLastName() <<
			"(ID: " << pkgs[j]->deliveredPkges[i]->getReceiver()->getId() << ")" <<
			endl;
		}
		cout << "-----------------------------------------------" << endl;

		cout << "Route on delivery: " << endl;
		cout << "-----------------------------------------------" << endl;
		cout << "Warehouse Location: " << warehouseLocation->getFullAddress() << endl;
		//print addresses of each package's reciever
		for (size_t i = 0; i < pkgs[j]->deliveredPkges.size(); i++){
			cout << pkgs[j]->deliveredPkges[i]->getReceiver()->getAddress()->getFullAddress() << endl;
		}
		cout << "Warehouse Location: " << warehouseLocation->getFullAddress() << endl;
		cout << "-----------------------------------------------" << endl;
	}
}

/*******************************************************************************************************************************************************
* Process: opens text file, reads each line and sets client object data members while initalizing new client object ever 4 lines
* Parameter: a reference to an empty clients type vector
* PostCondition: populates client objects into the vector
*********************************************************************************************************************************************************/
void Utility::readClients(vector<Client*>& clients){
	string line = "";
	ifstream readStream;
	readStream.open("//home//ubuntu//Downloads//finalProject//data//clients.txt");
	Client* client = new Client(0, "", "");

	int count = 1;
	//read each line
	while(getline(readStream,line))
	{
		//create new client
		if(line == "***"){
			clients.push_back(client);
			count = 0;
			client = new Client(0, "", "");
		}
		// set id, firstname and lastname
		else if(count == 1){
			vector<string> person;
			split(line, person);
			client->setId(parseInt(person[0]));
			client->setFirstName(person[1]);
			client->setLastName(person[2]);
		}
		//allocate memory for a new location object and point client's location to the memory allocated
		else if(count == 2){
			//init location have client address point to loc obj.
				location* address = new location(line);
				client->setAddress(address);
		}
		count++;
	}
	readStream.close();
}

/*******************************************************************************************************************************************************
* Process: opens text file, insert's each client's id, firstname, lastname, sent & delivered packages and close stream
* Parameter: empty clients type vector
* PostCondition: populates client objects into the vector
*********************************************************************************************************************************************************/
void Utility::writeClients(vector<Client*> &clients, const string fileName){
	ofstream outputStream;
	outputStream.open(fileName);

	//insert each client's data on output stream 
	for (int i = 0; i< clients.size(); i++){
		//id, first & last names and address
		outputStream << clients[i]->getId() << " " << clients[i]->getFirstName() << " " << clients[i]->getLastName() << endl;
		outputStream << clients[i]->getAddress()->getFullAddress()<< endl;

		//insert sent package Ids
		vector<int> sentPackages = clients[i]->getSentPackages();
		for(int j = 0; j < sentPackages.size(); j++){
			outputStream << sentPackages[j] << " ";
		}
		outputStream << endl;

		//insert received package Ids
		vector<int> recievedPackages = clients[i]->getRecievedPackages();
		for(int j = 0; j < recievedPackages.size(); j++){
			outputStream << recievedPackages[j] << " ";
		}
	    outputStream << endl << "***" << endl;
   	}
 	outputStream.close();
}

/*******************************************************************************************************************************************************
* Process: opens text file, reads warehouse's location and allocates memory for a location object then close stream
* Parameter: none
* PostCondition: returns a location
*********************************************************************************************************************************************************/
location* Utility::readWarehouseAddress(){
	string line = "";
	fstream readStream;
	readStream.open("//home//ubuntu//Downloads//finalProject//data//warehouse.txt");
	getline(readStream, line);
	
	location* address = new location(line);
	return address;
}

/*******************************************************************************************************************************************************
* Process: opens text file, allocates memory and stores data for each package where a package can be of overnight regular, package or twoDay package
* Parameter: file name, empty deque of the 3 types of packages and a referece to all the clients
* PostCondition: populates each deque with pakcages of it's type 
*********************************************************************************************************************************************************/
void Utility::createPackages(const string fileName, deque<OvernightPackage*>& nightPack, deque<TwoDayPackage*>& dayPack, deque<packages*>& regular, vector<Client*>& clients) {
	const double cPo{ 2.4 }, flatFee{ 5.9 }, additionalCpo{ 6.3 };	// constant values that are used for packages
	ifstream file(fileName);										// for reading from file

	// validate that input file stream could be opened for reading
	if (!file) {
		throw "The packages data could not be retrieved! closing program";
	}

	// while file buffer is not yet empty
	while (file){
		string packInfo{ "" };							// string for holding individual package info from textfile
		getline(file, packInfo);						// get line each time into string
		if(packInfo == ""){ 
			continue;
		}
		try{											// try-catch block in case of invalid packages
			Informations* info = readInfo(packInfo);					// create package based on information
			
			if (info->priority == 0) {					// create packages either for overnight or twoDay shipping
				// overnight package, based on priority
				OvernightPackage* p = new OvernightPackage(info->weight, cPo, additionalCpo, clients);	
				p->setIDS(info->senderID, info->receiverID, info->ID);			// restate sender and receiver data values
				p->setPriority(info->priority);								// set priority for package
				nightPack.push_back(p);									// store in overnight deque
			}
			else if (info->priority == 1){
				// twoDay package, based on priority
				TwoDayPackage* p = new TwoDayPackage(info->weight, cPo, flatFee, clients);
				p->setIDS(info->senderID, info->receiverID, info->ID);			// restate sender and receiver data values
				p->setPriority(info->priority);								// set priority for package
				dayPack.push_back(p);									// store in twoDay deque
			}
			else{
				// regular package, based on priority
				packages* p = new packages(info->weight, cPo, clients);
				p->setIDS(info->senderID, info->receiverID, info->ID);			// restate sender and receiver data values
				p->setPriority(info->priority);								// set priority for package
				regular.push_back(p);											// store in regular deque
			}

			//deallocate memory
			delete info;
		}
		catch (const char* e){							// catch exceptions that occur
			// catch block catch exception and output string
			cout << "\ninvalid format occurred \n" << e;
		}
		
	}
}

/*******************************************************************************************************************************************************
* Process: given a string splits it into chucks and returns a pointer to a structure holding package information 
* Parameter: a string of numbers separted by space
* PostCondition: returns a pointer to memory allocated the structure holding the data
*********************************************************************************************************************************************************/
Informations* Utility::readInfo(const string packageInformation) {
	// string stream for spliting information based on space char
	stringstream sSplit(packageInformation);
	string val{ "" };							// used for storing individual information
	vector<int> infoVect;						// used for storing all information
	bool digit{ true };							// for checking if digits are contained in string

	// while loop for retrieving individual information
	while (getline(sSplit, val, ' ')){

		// validate that integer value is indeed stored in string
		for (size_t i = 0; i < val.length(); i++){
			if ((!isdigit(val[i])) && (!ispunct(val[i]))) {
				throw "This package cannot be created due to invalid information format \n";
			}
		}
		int result{ 0 };					// input type
		stringParse(val, result);			// parse string to integer
		infoVect.push_back(result);			// store in vector
	}

	if (infoVect.size() < 7 || infoVect[2] > 2){			// for when there's not enough information in line of textfile
		throw "This package cannot be created due to insufficient amount of information or wrong priority input \n";
	}

	// create structure informations for infoVect information
	// Informations info;
	Informations* info = new Informations{};

	// store information
	info->ID = infoVect[0]; info->weight = infoVect[1]; info->priority = infoVect[2];
	info->senderID = infoVect[3]; info->receiverID = infoVect[4]; info->overDueLength = infoVect[5]; info->tracker = infoVect[6];
	return info;		// return structure
};
 
/*******************************************************************************************************************************************************
* Process: given all the selected packages, it goes through those clients that have received or sent the package and updates the information
* Parameter: a reference to the pointer of all the selected packages
* PostCondition: up-to-date client objects after delivery
*********************************************************************************************************************************************************/
void Utility::updateData(vector<packages*>& selectedPackages){
	for (int i = 0; i < selectedPackages.size(); ++i)
	{
		int id = selectedPackages[i]->getPackageID();
		selectedPackages[i]->getSender()->addSentPackage(id);
		selectedPackages[i]->getReceiver()->addRecievedPackage(id);;
	}
}

/*******************************************************************************************************************************************************
* Method: cmp
* Process: run sort on packages
* Parameter: const packages& first, const packages& second
* PreCondition: packages containers populated
* PostCondition: None
*********************************************************************************************************************************************************/
bool cmp(const packages* first, const packages* second) {
	return (first->getWeight() > second->getWeight());	// function for sorting packages based on that with greater weight
}


/*******************************************************************************************************************************************************
* Method: startSort
* Process: run mergesort on pack containers
* Parameter: deque<OvernightPackage>& nightPack, deque<TwoDayPackage>& dayPack
* PreCondition: packages containers populated
* PostCondition: ordered containers returned
*********************************************************************************************************************************************************/
void Utility::startSort(deque<OvernightPackage*>& nightPack, deque<TwoDayPackage*>& dayPack, deque<packages*>& pack) {
	sort(dayPack.begin(), dayPack.end(), cmp);		// sort day packages based on index
	sort(nightPack.begin(), nightPack.end(), cmp);	// sort night packages based on index
	sort(pack.begin(), pack.end(), cmp);			// sort regular packages based on index
}

/*******************************************************************************************************************************************************
* Method: checkID
* Process: validate package ID
* Parameter: const vector<int>& IDs, const int newID
* PreCondition: packages id provided
* PostCondition: None
*********************************************************************************************************************************************************/
void Utility::checkID(const vector<int>& IDs, const int newID) {
	// check to see if package id is already in use
	if (binary_search(IDs.begin(), IDs.end(), newID)) {
		// if package id already exists, throw exception
		throw "non-unique package ID, this package can't be created \n";
	}
}

/*******************************************************************************************************************************************************
* Process: opens file and calls each package's write package method with an output stream
* Parameter: filename and a reference to a vector of package pointers
* PostCondition: updates the packages file with the undelivered packages
*********************************************************************************************************************************************************/
void Utility::overwritePackagesToFile(const string fileName, vector<packages*>& pkgs) {
	ofstream output(fileName);
	// // make sure we can open the file
	if (!output) {
		cout <<  "the selected file can't be opened, therefore package information can't be written" << endl;
		return;
	}

	// for each package in pkgs
	for (size_t i = 0; i < pkgs.size(); i++) {
		// write package to textfile
		pkgs[i]->writePackage(output);
	}
}


/*******************************************************************************************************************************************************
* Process: opens file and calls each package's write package method with an output stream
* Parameter: filename and a reference to a vector of package pointers
* PostCondition: updates the delivered packages file with the new delivery
*********************************************************************************************************************************************************/
void Utility::appendPackagesToFile(const string fileName, vector<packages*>& pkgs){
	ofstream output(fileName, ios::app);	
	// // make sure we can open the file
	if (!output) {
		cout <<  "the selected file can't be opened, therefore package information can't be written" << endl;
		return;
	}

	// for each package in pkgs
	for (size_t i = 0; i < pkgs.size(); i++) {
		// write package to textfile
		pkgs[i]->writePackage(output);
	}
}
