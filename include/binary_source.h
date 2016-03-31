# ifndef BINARY_SOURCE_H_
# define BINARY_SOURCE_H_

# include <vector>
# include "netplus.h"

enum BinarySourceMode {Random, PseudoRandom, Deterministic};

using namespace std;

/* Generates a bit stream. Three types of sources are implemented (Random, PseudoRandom, Deterministic). In the Random mode the probability of generate a "0" is
going to be probabilityOfZero and probability of "1" is given by 1-probabilityOfZero. In the PseudoRandom mode, a PRBS sequence is generated with period
2^patternLength-1. In the Deterministic mode it is generated the sequence specified by bitStream.
If numberOfBits = -1 it generates an arbitrary large number of bits, otherwise the bit stream length equals numberOfBits.
The input parameter bitPerido specifies the bit period.
INPUT PARAMETERS:
BinarySourceMode type{ PseudoRandom };
double probabilityOfZero{ 0.5 };
int patternLength{ 7 };
string bitStream{ "01" };
long int numberOfBits{ -1 };
double bitPeriod{ 1.0 / 100e9 };
*/ 
class BinarySource : public Block {

	// Input parameters
	
	BinarySourceMode mode{ PseudoRandom };

	double probabilityOfZero{ 0.5 };
	int patternLength{ 7 };
	string bitStream{ "01" };

	long int numberOfBits{ -1 };
	double bitPeriod{ 1.0 / 100e9 };

	// State variables
	std::vector<int> acumul;
	int posBitStream{ 0 };

 public:

	// Methods
	BinarySource(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	
	bool runBlock(void);

	void setMode(BinarySourceMode m) {mode = m;}
	
	void setProbabilityOfZero(double pZero) { probabilityOfZero = pZero; }

	void setBitStream(string bStream) { bitStream = bStream; }

	void setNumberOfBits(long int nOfBits) { numberOfBits = nOfBits; }

	void setPatternLength(int pLength) { patternLength = pLength; }
	
	void setBitPeriod(double bPeriod);

};

# endif


