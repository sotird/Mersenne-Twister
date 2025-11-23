// Implementation of MersenneTwister.h
// Demetri Sotirelis
// 19 March 2025

// Originally Written by Makoto Matsumoto (1,2) and Takuji Nishimura (1)
// (1) Keio University (2) Max-Planck-Institut für Mathematik
// January 1998

#include "MersenneTwister.h"

#include <iostream>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <ratio>
#include <limits>
#include <vector>
#include <string>
#include <cmath>


//Precompile option to track statistics, uncomment to enable
#define statsistics

//Constructor
MersenneTwister::MersenneTwister(){
    n = 624;
    m = 397;
	w = 32;
	r = 31;
	UMASK = (0xffffffffUL << r);
	LMASK = (0xffffffffUL >> (w-r));
	a = 0x9908b0dfUL;
	u = 11;
	s = 7;
	t = 15;
	l = 18;
	b = 0x9d2c5680UL;
	c = 0xefc60000UL;
	f = 1812433253UL;
	
	// For this setup, most numbers generated fall between these bounds, this is meant to help normalize the output.
	// These numbers should be changed based on configuration of the program.
	upperBound = 1.00000e+19;
	lowerBound = 1.00000e+18;
	
	// Seed the Mersenne Twister hashmap
		// suggested initial seed = 19650218UL
	seed = 19650218;
    state_hashmap[0] = seed;
	for (int i=1; i<n; i++) {
	
		// Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.    
        seed = f * (seed ^ (seed >> (w-2))) + i; 
        state_hashmap[i] = seed; 
    }
    state_index = 0;
}


// Returns a random double within specified range
double MersenneTwister::MersenneRandomDouble(double range){

	double result = nRandDoubles(1).back();
	double percentage = ((result - this->lowerBound)/(this->upperBound - this->lowerBound));
	result = percentage * range;
	return result;
}
 
// Returns a random float within specified range
float MersenneTwister::MersenneRandomFloat(float range){

	double result = nRandDoubles(1).back();
	double percentage = ((result - this->lowerBound)/(this->upperBound - this->lowerBound));
	float resultfloat = percentage * range;
	return resultfloat;
}

// Returns a random int within specified range
int MersenneTwister::MersenneRandomInt(int range){
		
	double result = nRandDoubles(1).back();
	double percentage = ((result - this->lowerBound)/(this->upperBound - this->lowerBound));
	result = percentage * (range + 1); //Accounting for truncation
	int resultint = static_cast<int>(result);
	return resultint;	
}

// Write int n random numbers to a file with name std::string name in a given range
// Non-destructive, opens a file for appending, or creates it if it doesn't exist
// precision refers to what resolution numbers should be
// 		'd' --> double(s)
//		'f' --> float(s)
// 		'i' --> integer(s)
void MersenneTwister::MersennePrintRandomValues(std::string name, int num, double range, char precision){
	
	if(num == 0){return;}
	
	// Initialize output file
	std::ofstream randomNumberCache;
	randomNumberCache.open(name,std::ios::app);
	
	// If statement controlled by precision
	if(precision == 'd') {

		// print n random doubles in the given range
		for(int i = 0; i<num; i++){
			randomNumberCache << MersenneRandomDouble(range) << std::endl;
		}
		
	} else if(precision == 'f') {
	
		// print n random floats in the given range
		for(int i = 0; i<num; i++){
			
			randomNumberCache << this->MersenneRandomFloat(range) << std::endl;
		}
			
	} else if(precision == 'i') {
	
		// print n random floats in the given range
		for(int i = 0; i<num; i++){
			randomNumberCache << this->MersenneRandomInt(static_cast<int>(range)) << std::endl;
		}

	} else {
		std::cerr << "MersennePrintRandomValues(): Requires argument 3 (precision) to be one of \'d\' (double), \'f\' (float) or \'i\' (integer). No output produced." << std::endl;
	}
	
	// Close output file
	randomNumberCache.close();
}

		
// Generate the random integer using Mersenne Twister algorithm
unsigned long long int MersenneTwister::random(){
    
    int k = state_index;       		// point to current state location
                                     // 0 <= state_index <= n-1   always
    
    int j = k - (this->n-1);               // point to state n-1 iterations before
    if (j < 0){ j += this->n;}             // modulo n circular indexing

    unsigned long long int x = (state_hashmap[k] & this->UMASK) | (state_hashmap[j] & this->LMASK);
    unsigned long long int xA = x >> 1;
    if (x & 0x00000001UL) xA ^= this->a;
    
    j = k - ((this->n)-(this->m));         // point to state n-m iterations before
    if (j < 0) j += this->n;               // modulo n circular indexing
    
    x = state_hashmap[j] ^ xA; // compute next value in the state
    state_hashmap[k+1] = x;    // update new state value
    
    if (k >= n){k = 0;}				// modulo n circular indexing
    else{k++;}             
    state_index = k;
    
    unsigned long long int y = x ^ (x >> u);   			// tempering 
             			   y = y ^ ((y << s) & b);
            			   y = y ^ ((y << t) & c);
    unsigned long long int z = y ^ (y >> l);
    
    return z; 

}
		  
// Generate a pseudo-random seed based on a scrambling of the current system time at the nanosecond level.
unsigned long long int getSystemTimeSeed(){

	// Delay for pseudo-random
	for(int j = 0; j< 500; j++){;}
	
	// Measure system time
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::chrono::duration<long long int, std::nano> epoch_var = now.time_since_epoch();
	std::chrono::duration<long long int, std::nano> value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch_var);
	
	// Scramble system time
	unsigned long long int duration = value.count();
	duration = (duration<<16)/16384;
	duration *= duration;
	duration /= 1000;
	
	// Use the previously generated random number as a seed for the Mersenne Twister
	unsigned long long int seed = duration;
	return duration;
}

// Top level function that handles seeding and returns a random double in [1.00000e+18,1.00000e+19]
std::vector<double> MersenneTwister::nRandDoubles(int num){

	std::vector<double> results;
	unsigned long long int tempResult = 0;
	//Generate random numbers
	for(int i = 0; i<num; i++){
		
		tempResult = random();
		
		// Normalize output and track statistics, decrement i to skip an out of range result
		if((tempResult <= upperBound) && (tempResult >= lowerBound)){ results.push_back(tempResult); } 
		else { i--; }

	}
	
	// Function succesful
	return results;

}