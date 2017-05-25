#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "location.h"
#ifndef CLIENT_H
#define	CLIENT_H
using namespace std;


class Client
{
public:
    
    Client(const int _id, const string fName, const string lName);   
    int getId() const;
    void setId(int _id);
    string getFirstName() const;
    string getLastName() const;
    const vector<int>& getSentPackages() const;
    const vector<int>& getRecievedPackages() const; 
    void addSentPackage(const int id);
    void addRecievedPackage(const int id);
    void setFirstName(const string fName);
    void setLastName(const string lName);
    location*& getAddress();
    void setAddress(location* address);


private:
    int id;
    string firstName;
    string lastName;
    vector<int> sentPackages;
    vector<int> recievedPackages;
    location* address;  
};

#endif	

