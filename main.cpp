// Example code for MersenneTwister class
// Demetri Sotirelis
// 26 June 2025

#include "MersenneTwister.h"

int main(int argc, char*argv[]){

	//initialize the data type
	MersenneTwister GimmieRandomNumbers;
	
	//Generate a random double between 0 and 100.0124
	double MyRandomDouble = GimmieRandomNumbers.MersenneRandomDouble(100.0124);
	
	//Generate a random float between 0 and 100.0124
	float MyRandomFloat = GimmieRandomNumbers.MersenneRandomFloat(100.0124);
	
	//Generate a random int between 0 and 10;
	int MyRandomInt = GimmieRandomNumbers.MersenneRandomInt(10);
	
	//Print a series of 100 random integers on [0,99] to a file
	GimmieRandomNumbers.MersennePrintRandomValues("Output.txt", 1000000, 99, 'i'); // 'i', 'f', or 'd'
	
	return 0;
	
}