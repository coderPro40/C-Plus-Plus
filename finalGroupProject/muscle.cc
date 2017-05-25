#include <string>
#include "muscle.h"
#include "FindTime.h"
#include <cstdlib>
#include <ctime>
#include <deque>
#include <limits.h>
#include <iostream>
#include <algorithm>

using namespace std;

// structs
struct Vertex{		// hold objects for heap
	int minutes;		// minutes for vertex object
	int index;		// index for vertex object
};

// function prototypes
bool predicate(const Vertex* i, const int j);
bool secondPred(packages*i, const string j);

// decide which package to choose
//void crtPkg(const deque<OvernightPackage*>& overnight, const deque<TwoDayPackage*>& twoDay, const deque<packages*>& regular, const int counter, deque<packages*>& currentPkg);
void updatePQ(vector<int>& key, vector<int>& pred, deque<Vertex*>& PQ, const int ind1, const int val, const int ind2 = -1);			// use to update algorithm's informations
void setValues(const int sizer, vector<int>& key, vector<int>& pred, deque<Vertex*>& PQ);	// update priority queue based on size of package type
void selectPackages(location*& warehouseLocation, Truck*& truck);			// for selecting packages for truck driver
bool cmp(const Vertex* first, const Vertex* second);			// comparison function based on minutes
bool cmpTwo(const Truck* first, const Truck* second);			// comparison function based on minutes
bool compString(packages* first, packages* second);				// comparison function based on direction
bool isIndex(const Vertex* val);								// comparison function based on index
void cpypkgs(vector<packages*>& tmp, Truck*& truck);			// copy from pkgs to truck
void makePending(Truck*& truck, vector<packages*>& pending);	// copy pending packages to next truck
void storePending(vector<packages*>& pending, Truck*& truck);	// copy undelivered values from truck to pending
int getIndex(int val = -1);										// get index for function
bool compPred(packages* first, packages* second);				// for comparing values of delivered and pending
void validate(vector<Truck*>& trucks, vector<packages*>& pending, location*& warehouse);	// for validating that no route exist for deliver of package

// new findTime object
findTime* Time = new findTime();

void Muscle::deliveryTrucks(location*& warehouseLocation, vector<packages*>& pkgs, vector<packages*>& selected, vector<packages*>& pending, vector<Truck*>& trucks) {
	// this function is used to create truck objects to deliver packages to different parts of the city
	vector<string> locations{ "W", "NW", "N", "NE", "E", "SE", "S", "SW" };		// all locations for travel
	size_t counter{ 0 };	// use this to keep track of selected packages vector

	// create truck objects and assign locations to each truck
	//copy(pkgs.begin(), pkgs.end(), temp.begin());								// alternative for just one time sorting
	sort(pkgs.begin(), pkgs.end(), compString);									// sort based on directions
	for (size_t i = 0; i < locations.size(); i++){
		Truck* truck = new Truck;			// create truck structure
		truck->ID = i;						// set truck id
		truck->direction = locations[i];	// set direction for truck

		// for updating trucks for delivery
		cpypkgs(pkgs, truck);						// copy valid package to truck
		//makePending(truck, pending);				// copy values from pending array to next truck
		selectPackages(warehouseLocation, truck);	// update packages and deliveries for truck
		storePending(pending, truck);				// store pending values from truck to pending vector
		trucks.push_back(truck);					// store in trucks vector
	}

	// for if there're still packages pending
	validate(trucks, pending, warehouseLocation);

	// copy values from delivered packages to selected vector
	for (size_t i = 0; i < trucks.size(); i++){
		counter = selected.size();			// update size of counter
		if (trucks[i]->deliveredPkges.size() > 0) {		// store delivered items in selected vector
			selected.resize(selected.size() + trucks[i]->deliveredPkges.size());			// resize selected vector
			copy(trucks[i]->deliveredPkges.begin(), trucks[i]->deliveredPkges.end(), selected.begin() + counter);	// copy values to selected vector
		}
	}
}

void selectPackages(location*& warehouseLocation, Truck*& truck){
	// needed to make this function as large as it is in order to account for any and all packages in an efficient and timely manner
	vector<int> key, pred;			// for current best time for package delivery and for package predecessor
	deque<Vertex*> PQ;				// priority queue for algorithm
	Truck* selected = new Truck;	// selected truck temp

	// ensure that not all packages are empty
	if(truck->storedPkgs.size() == 0){
		return;
	};

	// shortest route algorithm to determine selected packages
	selected = truck;						// copy truck values to a temp for each run
	truck->deliveredPkges.clear();			// clean truck up for each run so as to prevent duplicates
	const int truckWeight{ 500 };
	bool running{ true }, wHouse{ true };	// for controlling loop of prim's algorithm
	
	// for controlling package counter
	size_t counter{ 0 }, curSize{ 0 }, totTime{ 0 }, totTrkWgt{ 0 };	

	// choose which package to use for the algorithm
	curSize = selected->storedPkgs.size();	// get size of two day package container
	setValues(curSize, key, pred, PQ);		// update values based on packages

	// for initially updating information of algorithm
	updatePQ(key, pred, PQ, curSize, 0);

	// loop for all adjacent vertices
	while (PQ.size() != 0 && running){			// as long as pq isn't empty
		Vertex* ver = PQ.front();				// new vertex to use to find adjacent vertices
		PQ.pop_front();					// update PQ
		make_heap(PQ.begin(), PQ.end(), cmp);	// remake heap
		for (size_t i = 0; i < curSize; i++){	// each vertex in the pq
			unsigned int deliveryTime{ 0 };
			if (wHouse){			// for when warehouse not yet left
				deliveryTime = Time->GetTime(warehouseLocation, selected->storedPkgs[i]->getReceiver()->getAddress());
			}
			else{
				// otherwise go from current location to next location
				deliveryTime = Time->GetTime(selected->storedPkgs[ver->index]->getReceiver()->getAddress(), selected->storedPkgs[i]->getReceiver()->getAddress());
			}
			if (key[i] > deliveryTime){		// if current hour of index is greate than hour from current vertex to new vertex
				// update packages info continuously
				updatePQ(key, pred, PQ, i, deliveryTime, ver->index);
			}
		}
		if (wHouse) {			// iterate for other package locations to run on algorithm
			wHouse = false;
		}

		// use this for tracking the time limit and the weight limit
		if (ver->minutes > 0){		// for keeping from going out of range
			// used to update weight
			totTrkWgt += selected->storedPkgs[ver->index]->getWeight();
			totTime += ver->minutes;	// for getting time of each chosen package

			// decide whether to stop loop or continue 
			if ((totTime >= (8 * 60)) || (totTrkWgt >= truckWeight)) {
				// stop loop if time limit or weight is reached
				running = false;
			}
			else {
				truck->weight = totTrkWgt;	// update weight variable
				truck->minutes = totTime;	// update minutes variable
				truck->deliveredPkges.push_back(selected->storedPkgs[ver->index]);	// store selected package in vector
			}
		}
	}
	/*counter += 1;*/	// continue finding shortest paths if hours and weight limits not exceeded
}

void setValues(const int sizer, vector<int>& key, vector<int>& pred, deque<Vertex*>& PQ) {
	const int maximum{ numeric_limits<int>::max() }, init{ -1 };	// initial values for key and pred
	key.clear(); pred.clear(), PQ.clear();							// first clear container of previous values
	for (size_t i = 0; i < sizer; i++) {
		key.push_back(maximum);			// initially all keys are set to infinity
		pred.push_back(init);			// initially all preds are set to -1
		Vertex* ver = new Vertex{};						// create object
		ver->minutes = maximum;			// store minutes for object
		ver->index = i;					// store index of object
		PQ.push_back(ver);				// store in PQ
	}
	key.push_back(maximum);				// for warehouse index
	pred.push_back(init);				// for warehouse index
	Vertex* ver = new Vertex{};							// create object
	ver->minutes = maximum;				// store minutes for object
	ver->index = sizer;					// store index of object
	PQ.push_back(ver);						// store in PQ
	make_heap(PQ.begin(), PQ.end(), cmp);	// create heap based on minutes
}

void validate(vector<Truck*>& trucks, vector<packages*>& pending, location*& warehouse) {
	// truck with the smallest time spend in the group of trucks
	sort(trucks.begin(), trucks.end(), cmpTwo);
	for (size_t i = 0; i < trucks.size() && pending.size() > 0; i++){			// for moving packages from pending to available trucks
		if (trucks[i]->weight < 50) {
			makePending(trucks[i], pending);			// make pending for truck
			selectPackages(warehouse, trucks[i]);		// run optimal route algorithm again
			storePending(pending, trucks[i]);			// store pending if any packages remain
		}
	}
}

bool cmp(const Vertex* first, const Vertex* second) {
	return (first->minutes > second->minutes);	// function for creating maximum PQ
}

bool cmpTwo(const Truck* first, const Truck* second) {
	return (first->minutes < second->minutes);	// function for choosing minimum truck
}

bool isIndex(const Vertex* val) {
	const int index = getIndex();
	return (val->index == index);	// function for creating maximum PQ
}

int getIndex(int val) {
	// for is index function
	static int comparison{ 0 };
	if (val != -1){
		comparison = val;			// ensure that indexes are saved for future use
	}
	return comparison;
}

void updatePQ(vector<int>& key, vector<int>& pred, deque<Vertex*>& PQ, const int ind1, const int val, const int ind2) {
	int use = getIndex(ind1);						// get index of warehouse vertex for PQ
	vector<int> wower{ ind1 };						// array of element to search

	// iterator to make sure element exist in pq
	deque<Vertex*>::iterator it = search(PQ.begin(), PQ.end(), wower.begin(), wower.end(), predicate);
	if (it != PQ.end()){						// make sure vertex still in pq
		int curIndex{ 0 };						// keeping track of PQ index
		key[ind1] = val;						// update key of warehouse vertex
		curIndex = find_if(PQ.begin(), PQ.end(), isIndex) - PQ.begin();
		PQ[curIndex]->minutes = val;			// update minutes for warehouse vertex
		make_heap(PQ.begin(), PQ.end(), cmp);	// remake heap
		pred[ind1] = ind2;						// update pred of current index
	}
}

bool predicate(const Vertex* i, const int j) {
	return (i->index == j);						// predicate for checking if element exist in pq
}

//void crtPkg(const deque<OvernightPackage*>& overnight, const deque<TwoDayPackage*>& twoDay, const deque<packages*>& regular, const int counter, deque<packages*>& currentPkg){
//	currentPkg.clear();		// first clear current packages of previous values
//	if (counter == 0){
//		// for first counter, create trackers for overnight packages
//		currentPkg.resize(overnight.size());
//		copy(overnight.begin(), overnight.end(), currentPkg.begin());
//	}
//	else if (counter == 1){
//		// for first counter, create trackers for two day packages
//		currentPkg.resize(twoDay.size());
//		copy(twoDay.begin(), twoDay.end(), currentPkg.begin());
//	}
//	else{
//		// for first counter, create trackers for regular packages
//		currentPkg.resize(regular.size());
//		copy(regular.begin(), regular.end(), currentPkg.begin());
//	}
//}

void cpypkgs(vector<packages*>& tmp, Truck*& truck) {
	vector<string> wower{ truck->direction };						// array of element to search

	// iterator to make sure element exist in temp
	vector<packages*>::iterator it = search(tmp.begin(), tmp.end(), wower.begin(), wower.end(), secondPred);
	if (it != tmp.end()) {
		bool running{ true };							// control loop
		for (size_t i =  (it - tmp.begin()); i < tmp.size() && running; i++) {		// for each package in pkgs array
			if (truck->direction == tmp[i]->getReceiver()->getAddress()->getDirection()) {
				truck->storedPkgs.push_back(tmp[i]);	// if package has same direction as truck
			}
			else{
				running = false;						// end loop if directions are different
			}
		}
	}
}

bool secondPred(packages*i, const string j) {
	// return true if direction exist in array
	return(i->getReceiver()->getAddress()->getDirection() == j);
}

bool compString(packages* first, packages* second) {
	// comparison based on direction of packages
	vector<string> locations{ "W", "NW", "N", "NE", "E", "SE", "S", "SW" };
	int one{ 0 }, two{ 0 };		// initial value of one and two
	
	// place values to directions
	for (size_t i = 0; i < locations.size(); i++){
		if (first->getReceiver()->getAddress()->getDirection() == locations[i]) {
			one = i;			// store int value to one 
		}
		if (second->getReceiver()->getAddress()->getDirection() == locations[i]){
			two = i;			// store int value to two
		}
	}
	return(one > two);
}

void makePending(Truck*& truck, vector<packages*>& pending) {
	if (pending.size() > 0){							// as long as the size is greater than 0
		for (size_t i = pending.size(); i > 0; i--) {	// for each pending package
			truck->storedPkgs.push_back(pending[i-1]);			// store pending package
			pending.pop_back();									// remove pending package
		}
	}
}

void storePending(vector<packages*>& pending, Truck*& truck) {
	while (truck->storedPkgs.size() > truck->deliveredPkges.size()){
		for (size_t i = 0; i < truck->storedPkgs.size(); i++){	// as long as the size cap isn't met
			const size_t lPos{ truck->storedPkgs.size() - 1 };	// size of stored packages
			swap(truck->storedPkgs[i], truck->storedPkgs[lPos]);// swap last position with current position
			vector<packages*>::iterator it;						// iterator to keep track of values for comparison
			vector<packages*> wower{ truck->storedPkgs[lPos] };	// array of element to search
			it = search(truck->deliveredPkges.begin(), truck->deliveredPkges.end(), wower.begin(), wower.end(), compPred);
			if (it == truck->deliveredPkges.end()){
				pending.push_back(truck->storedPkgs[lPos]);		// store package in array if valid, has not been delivered
				truck->storedPkgs.pop_back();					// remove last position position to avoid duplicates
			}
		}
	}
	//old trial
	//for (size_t i = 0; i < truck->storedPkgs.size(); i++) {	// as long as the size cap isn't met
	//	vector<packages*>::iterator it;						// iterator to keep track of values for comparison
	//	vector<packages*> wower{ truck->storedPkgs[i] };	// array of element to search
	//	it = search(truck->deliveredPkges.begin(), truck->deliveredPkges.end(), wower.begin(), wower.end(), compPred);
	//	if (it == truck->deliveredPkges.end()) {
	//		pending.push_back(truck->storedPkgs[i]);		// store package in array if valid, has not been delivered
	//		truck->storedPkgs.erase(truck->storedPkgs.begin() + i);	// erase position to avoid duplicates
	//	}
	//}
}

bool compPred(packages* first, packages* second) {
	// compare based on package id of both lists
	return(first->getPackageID() == second->getPackageID());
}