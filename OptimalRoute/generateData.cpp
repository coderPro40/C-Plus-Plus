#include "generateData.h"

using namespace std;

// function prototypes
bool comparison(Client* one, Client* two);												// comparison for sorting and binary searching
bool pred(const Client* i, const size_t j);												// comparison for value and searching of client's array
bool predTwo(const Informations* i, const size_t j);									// comparison for values and searching of infos' array
void createAddress(const vector<vector<string>>& holder, Client*& client);				// for creating address for client
void generateRandPkgs(vector<Client*>& clients, const size_t pkgsSize, Utility*& util);	// for generating random packages
void trackUpdate(Informations*& info);													// for updating tracker and overdue length
void readPkgs(vector<Informations*>& infos, Utility*& util);							// for reading from packages
void writePkgs(vector<Informations*>& infos);											// for writing to packages

void GenerateData::generateRandClients(Utility*& util, const size_t number){
	random_device rd;					// for randomization number
	mt19937 mersenne(rd());				// initialize mersenne twister
	vector<Client*> clients;			// clients vector that store clients objects
	const size_t newClientSize{ number };	// size of new clients to create
	const string clientsFile{ "//home//ubuntu//Downloads//finalProject//data//clients.txt" };
	util->readClients(clients);			// read in current clients

	// for total size of client's array at end of procedure
	const size_t totSize{ newClientSize + clients.size() };

	// vector strings of first and last names and client informations
	const vector<string> firstName{ "budz", "pain", "konan", "nagato", "itachi", "tobi", "madara", "naruto", "danzou", "kakashi" };
	const vector<string> lastName{ "Roberts", "Kennedy", "Alonso", "Gates", "Norman", "Fields", "Potter", "Carter", "Jackson", "Thomas" };
	const vector<string> directions{ "N", "E", "S", "W", "NE", "NW", "SE", "SW" };		// vector for directions
	const vector<string> street{ "Main", "Central" };				// street possibilities for N, E, S, W
	const vector<string> streetTp{ "st", "ave" };					// street types
	const vector<vector<string>> holder{ firstName, lastName, directions, street, streetTp };	// transfer to function
	Client* client = new Client(0, "", "");					// create client object	
	while (clients.size() < (totSize)){
		const size_t id{ (mersenne() % (totSize + 1)) + 1 };	// random value derived this turn
		vector<size_t> storage{ id };						// store id value for processing
		vector<Client*>::iterator it = search(clients.begin(), clients.end(), storage.begin(), storage.end(), pred);
		if (it == clients.end()){	// binary search to see if exist already in array
			const size_t fN{ mersenne() % firstName.size() }, lN{ mersenne() % lastName.size() };
			client = new Client(id, firstName[fN], lastName[lN]);				// update new object
			createAddress(holder, client);										// create address for client object
			clients.push_back(client);											// add to clients vector
		}
	}
	sort(clients.begin(), clients.end(), comparison);							// sort clients based on id
	util->writeClients(clients, clientsFile);									// write clients file to textfile
	generateRandPkgs(clients, number, util);									// create random packages based on clients
}

void generateRandPkgs(vector<Client*>& clients, const size_t pkgsSize, Utility*& util) {
	random_device rd;					// for randomization number
	mt19937 mersenne(rd());				// initialize mersenne twister
	vector<Informations*> infos;		// hold all information
	readPkgs(infos, util);				// update infos vector

	// for total size of client's array at end of procedure
	const size_t totSize{ pkgsSize + infos.size() };

	while (infos.size() < totSize){
		const size_t id{ (mersenne() % (totSize + 1)) + 1 };	// random value derived this turn
		vector<size_t> storage{ id };						// store id value for processing
		vector<Informations*>::iterator it = search(infos.begin(), infos.end(), storage.begin(), storage.end(), predTwo);
		if (it == infos.end()) {
			// create new info structure
			Informations* info = new Informations;

			// for weight and priority of random package
			const size_t weight{ (mersenne() % (30)) + 1 }, priority{ mersenne() % (3) };
			size_t sendID{ 0 }, receiveID{ 0 };				// for sender id and receiver id of random package
			
			// validation loop
			do{
				sendID	= (mersenne() % (clients.size() + 1)) + 1;
				receiveID = (mersenne() % (clients.size() + 1)) + 1;
			} while (sendID == receiveID);

			// update info members
			info->ID = id; info->weight = weight; info->priority = priority; info->senderID = sendID; info->receiverID = receiveID;
			trackUpdate(info);								// for updating tracker and overdue length
			infos.push_back(info);							// store in infos vector
		}
	}
	writePkgs(infos);										// write to packages file
}

void createAddress(const vector<vector<string>>& holder, Client*& client){
	random_device rd;					// for randomization number
	mt19937 mersenne(rd());				// initialize mersenne twister
	stringstream parse;					// for converting to strings
	stringstream parse2;					// for converting to strings

	// client informations
	const vector<string> firstName{ holder[0] }, lastName{ holder[1] }, directions{ holder[2] }, street{ holder[3] }, streetTp{ holder[4] };
	string address{ "" }, hNo{ "" }, hBlks{ "" };						// initially empty string of address

	const size_t dir{ mersenne() % directions.size() };
	if ((directions[dir] == "N") || (directions[dir] == "S")){			// for Central Avenue objects
		const size_t houseNo{ (mersenne() % 449) + 1 };
		parse << houseNo;
		parse >> hNo;
		address = hNo + " " + street[1] + " " +
			streetTp[1] + " " + directions[dir];
	}
	else if ((directions[dir] == "E") || (directions[dir] == "W")){		// for Main Street objects
		const size_t houseNo{ (mersenne() % 449) + 1 };
		parse << houseNo;
		parse >> hNo;
		address = hNo + " " + street[0] + " " +
			streetTp[0] + " " + directions[dir];
	}
	else{																// for other directions
		const size_t blocks{ (mersenne() % 3) + 1 };
		const size_t houseNo{ (mersenne() % ((4 - blocks) * 99)) + 1 };
		parse << blocks;
		parse >> hBlks;
		parse2 << houseNo;
		parse2 >> hNo;
		const size_t str{ mersenne() % streetTp.size() };
		address = hNo + " " + hBlks + " " +
			streetTp[str] + " " + directions[dir];
	}
	location* locate = new location(address);							// create new location for address
	client->setAddress(locate);											// set new address for client
}

bool comparison(Client* one, Client* two){
	// values for binary search
	return(one->getId() < two->getId());
}

bool pred(const Client* i, const size_t j) {
	return (i->getId() == j);											// predicate for checking if element exist in clients
}

bool predTwo(const Informations* i, const size_t j) {
	return (i->ID == j);											// predicate for checking if element exist in infos
}

void trackUpdate(Informations*& info) {
	info->overDueLength = 0;
	if (info->priority == 0) {							// for overnight packages
		info->tracker = 0;
	}
	else if (info->priority == 1) {						// for two day packages
		info->tracker = 2;
	}
	else {												// for regular packages
		info->tracker = -1;
	}
}

void readPkgs(vector<Informations*>& infos, Utility*& util) {
	string line{ "" };					// for reading line
	ifstream readStream;
	readStream.open("//home//ubuntu//Downloads//finalProject//data//packages.txt");

	//read each line
	while (getline(readStream, line)) {
		Informations* info = util->readInfo(line);	// read line into info structure
		infos.push_back(info);						// store in infos vector
	}
}

void writePkgs(vector<Informations*>& infos) {
	ofstream outputStream;
	outputStream.open("//home//ubuntu//Downloads//finalProject//data//packages.txt");

	for (size_t i = 0; i < infos.size(); i++){
		outputStream << infos[i]->ID << " " << infos[i]->weight << " " << infos[i]->priority <<
			" " << infos[i]->senderID << " " << infos[i]->receiverID << " " << infos[i]->overDueLength <<
			" " << infos[i]->tracker << endl;
	}
}
