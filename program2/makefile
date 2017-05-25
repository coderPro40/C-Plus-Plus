CXX = g++
CC = g++
# require -std=gnu++11 to run makefile
FLAGS = -std=gnu++11

curling: curling.cpp curlingStatistic.o 
	$(CXX) $(FLAGS) curling.cpp curlingStatistic.o -o curling

# unnessary file doesn't run
curling.o: curling.cpp
	$(CXX) $(FLAGS) -c curling.cpp

curlingStatistic.o: curlingStatistic.cpp curlingStatistic.h
	$(CXX) $(FLAGS) -c curlingStatistic.cpp

clobber:
	rm -f *.o *.exe core curling

