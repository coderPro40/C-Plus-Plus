CXX = g++
CC = g++
CFLAGS = -std=c++11

default: delivery

delivery: location.o FindTime.o client.o utility.o packages.o OvernightPackage.o TwoDayPackage.o muscle.o generateData.o main.cc
	$(CC) $(CFLAGS) location.o FindTime.o client.o utility.o packages.o OvernightPackage.o TwoDayPackage.o muscle.o generateData.o main.cc -o delivery
client.o: client.cc client.h
	$(CC) $(CFLAGS) -c client.cc
packages.o: packages.cpp packages.h
	$(CC) $(CFLAGS) -c packages.cpp
OvernightPackage.o: OvernightPackage.cpp OvernightPackage.h
	$(CC) $(CFLAGS) -c OvernightPackage.cpp
TwoDayPackage.o: TwoDayPackage.cpp TwoDayPackage.h
	$(CC) $(CFLAGS) -c TwoDayPackage.cpp
muscle.o: muscle.cc muscle.h
	$(CC) $(CFLAGS) -c muscle.cc
generateData.o: generateData.cpp generateData.h
	$(CC) $(CFLAGS) -c generateData.cpp
utility.o: utility.cc utility.h
	$(CC) $(CFLAGS) -c utility.cc
location.o: location.cc location.h
	$(CC) $(CFLAGS) -c location.cc
FindTime.o: FindTime.cc FindTime.h
	$(CC) $(CFLAGS) -c FindTime.cc
clobber:
	rm -f *.o *.exe core delivery
