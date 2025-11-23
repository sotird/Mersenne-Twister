// A modern implementation of the Mersenne Twister algorithm
// using hashmaps, this algorithm was originally written by
// Makoto Matsumoto (1,2) and Takuji Nishimura (1)
// (1) Keio University (2) Max-Planck-Institut für Mathematik
// January 1998

// This class wrapper also modified the original algorithm 
// to use hashmaps, 64 bit integers, and a system time-based seed 
// by Demetrios Sotirelis 13 March 2025
// Updated 26 June 2025

// Apple clang version 16.0.0 (clang-1600.0.26.6)
// Target: arm64-apple-darwin24.0.0
// Thread model: posix

// Currently rigged to generate n = argv[1] values over [1.00000e+18 1.00000e+19] 
// Takes one argument as an integer for the amount of random numbers to generate
// Demetri Sotirelis
// 19 March 2025

#ifndef MERSENNETWISTER_H
#define MERSENNETWISTER_H

#include <unordered_map>
#include <vector>
#include <string>

class MersenneTwister {
	public:
	
		 // Constructor
		 // Precision can be double, float, or int. Depending on
		 // the function used.
		 MersenneTwister();
		
		// Returns respective data types in inclusive range [0,Range]
		double MersenneRandomDouble(double range);
		float  MersenneRandomFloat(float range);
		int    MersenneRandomInt(int range);

		// Writes n random doubles within range to a file, name specified by std::string name
		void MersennePrintRandomValues(std::string name, int n, double range, char precision = 'd');

	private:
		
		//Stores the range of the random number generator, use multiples of 10.
		double range;
		
		//	MT Data used throughout the twister algorithm
		std::unordered_map<int, unsigned long long int> state_hashmap;	// the array for the state vector
    	int state_index;  // index into state vector array, 0 <= state_index <= n-1   always              									
		
		
		//Functions concerning the implementation the Mersenne Twister
		  // Generate the actual random integer
		  unsigned long long int random();
		  
		  // Top level function that handles seeding and returns a random double in [1.00000e+18,1.00000e+19]
		  std::vector<double> nRandDoubles(int n);
		 
		  // Call this function to use a scrambling of the computer system time as a seed
		  
		  // Call this function to use the recommended static seed, 
		  // the same every time. This is the seed the generator uses by default.
		  
		  // Call this function to use your own custom seed (integer)
		 
		//Variables  
		unsigned int n,m,w,r,u,s,t,l;
		unsigned long long int UMASK,LMASK;
		unsigned long long int a,b,c,f;
		unsigned long long int upperBound, lowerBound;
		
		unsigned long long int seed;
};

#endif // MERSENNETWISTER_H
