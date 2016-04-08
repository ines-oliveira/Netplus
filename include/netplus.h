# ifndef PROGRAM_INCLUDE_NETPLUS_H_
# define PROGRAM_INCLUDE_NETPLUS_H_

# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <complex>

using namespace std;

typedef unsigned int t_binary;
typedef double t_real;
typedef complex<double> t_complex;

const int MAX_NAME_SIZE = 256;  // Maximum size of names
const long int MAX_Sink_LENGTH = 100000;  // Maximum Sink Block number of values
const int MAX_BUFFER_LENGTH = 10000;  // Maximum Signal buffer length
const int MAX_TOPOLOGY_SIZE = 100;  // Maximum System topology size 
const int MAX_TAPS = 1000;  // Maximum Taps Number
const double PI = 3.1415926535897932384;


//########################################################################################################################################################
//############################################################## SIGNALS DECLARATION AND DEFINITION ######################################################
//########################################################################################################################################################

// Root class for signals
class Signal {
							
public:

	Signal(string fName) {fileName = fName; };		// Signal constructor
	Signal() {};									// Signal constructor

	~Signal(){ delete buffer; };					// Signal destructor

	void close();									// Empty the signal buffer and close the signal file
	int space();									// Returns the signal buffer space
	int ready();									// Returns the number of samples in the buffer ready to be processed
	void writeHeader();								// Opens the signal file in the default signals directory, \signals, and writes the signal header
	void writeHeader(string signalPath);			// Opens the signal file in the signalPath directory, and writes the signal header

	template<typename T>							// Puts a value in the buffer
	void bufferPut(T value) {
		(static_cast<T *>(buffer))[inPosition] = value;
		if (bufferEmpty) bufferEmpty = false;
		inPosition++;
		if (inPosition == bufferLength) { 
			inPosition = 0; 
			if (firstValueToBeSaved <= bufferLength) {
				char *ptr = (char *) buffer;
				ptr = ptr + (firstValueToBeSaved - 1)*sizeof(T);
				ofstream fileHandler("./signals/" + fileName, ios::out | ios::binary | ios::app);
				fileHandler.write(ptr, (bufferLength - (firstValueToBeSaved - 1 ))* sizeof(T));
				fileHandler.close();
				firstValueToBeSaved = 1;
			}
			else {
				firstValueToBeSaved = firstValueToBeSaved - bufferLength;
			}
		}
		if (inPosition == outPosition) bufferFull = true;
	};

	string type;									// Signal type

	int bufferLength{ 512 };						// Buffer length
	void *buffer{ NULL };							// Pointer to buffer
	int inPosition{ 0 };							// Next position to the inputed values
	int outPosition{ 0 };							// Next position to the outputed values
	bool bufferEmpty{ true };						// Flag bufferEmpty
	bool bufferFull{ false };						// Flag bufferFull

	string fileName{ "" };							// Name of the file where data values are going to be saved
	long int firstValueToBeSaved{ 1 };				// First value (>= 1) to be saved
	long int numberOfValuesToBeSaved{ -1 };			// Number of values to be saved, if -1 all values are going to be saved
	long int numberOfSavedValues{ 0 };				// Number of saved values

	long int count;									// Number of values that have already entered in the buffer

	double symbolPeriod;							// Signal symbol period (it is the inverse of the symbol rate)
	double samplingPeriod;							// Signal sampling period (it is the time space between two samples)

};


class TimeDiscrete : public Signal {
public:
	TimeDiscrete(string fName) { fileName = fName; }
	TimeDiscrete(){}
};


class TimeDiscreteAmplitudeDiscrete : public TimeDiscrete {
public:
	TimeDiscreteAmplitudeDiscrete(string fName) { fileName = fName; }
	TimeDiscreteAmplitudeDiscrete(){}
};


class TimeDiscreteAmplitudeContinuous : public TimeDiscrete {
public:
	TimeDiscreteAmplitudeContinuous(string fName) { fileName = fName; }
	TimeDiscreteAmplitudeContinuous(){}
};


class TimeDiscreteAmplitudeDiscreteReal : public TimeDiscreteAmplitudeDiscrete {
public:
	TimeDiscreteAmplitudeDiscreteReal(string fName) {type = "TimeDiscreteAmplitudeDiscreteReal"; fileName = fName; buffer = new t_real[bufferLength]; }
	TimeDiscreteAmplitudeDiscreteReal(string fName, int bLength) { type = "TimeDiscreteAmplitudeDiscreteReal"; fileName = fName; bufferLength = bLength;  buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeDiscreteReal(int bLength) { type = "TimeDiscreteAmplitudeDiscreteReal"; bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeDiscreteReal(){ if (type == "") type = "TimeDiscreteAmplitudeDiscreteReal"; if (buffer == nullptr) buffer = new t_real[bufferLength]; }

	t_real bufferGet() {
		t_real value = static_cast<t_real *>(buffer)[outPosition];
		if (bufferFull) bufferFull = false;
		outPosition++;
		if (outPosition == bufferLength) outPosition = 0;
		if (outPosition == inPosition) bufferEmpty = true;
		return (value);
	}


};


class TimeDiscreteAmplitudeDiscreteComplex : public TimeDiscreteAmplitudeDiscrete {
public:
	TimeDiscreteAmplitudeDiscreteComplex(string fName) { fileName = fName; }
	TimeDiscreteAmplitudeDiscreteComplex() {}
};


class Binary : public TimeDiscreteAmplitudeDiscrete {
	
public:
	Binary(string fName) { type = "Binary";  fileName = fName; buffer = new t_binary[bufferLength]; }
	Binary(string fName, int bLength) { type = "Binary";  fileName = fName; bufferLength = bLength; buffer = new t_binary[bLength]; }
	Binary(int bLength) { type = "Binary";  bufferLength = bLength; buffer = new t_binary[bLength]; }
	Binary() { if (type == "") type = "Binary"; if (buffer == nullptr) buffer = new t_binary[bufferLength]; }

	t_binary bufferGet() {
		t_binary value = static_cast<t_binary *>(buffer)[outPosition];
		if (bufferFull) bufferFull = false;
		outPosition++;
		if (outPosition == bufferLength) outPosition = 0;
		if (outPosition == inPosition) bufferEmpty = true;
		return (value);
	}

};


class TimeDiscreteAmplitudeContinuousReal : public TimeDiscreteAmplitudeContinuous {
public:
	TimeDiscreteAmplitudeContinuousReal(string fName) { type = "TimeDiscreteAmplitudeContinuousReal"; fileName = fName; buffer = new t_real[bufferLength]; }
	TimeDiscreteAmplitudeContinuousReal(string fName, int bLength) { type = "TimeDiscreteAmplitudeContinuousReal"; fileName = fName; bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeContinuousReal(int bLength) { type = "TimeDiscreteAmplitudeContinuousReal"; bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeContinuousReal(){ if (type=="") type = "TimeDiscreteAmplitudeContinuousReal"; if (buffer == nullptr) buffer = new t_real[bufferLength]; }
};


class TimeDiscreteAmplitudeContinuousComplex : public TimeDiscreteAmplitudeContinuous {
public:
	TimeDiscreteAmplitudeContinuousComplex(string fName) { type = "TimeDiscreteAmplitudeContinuousComplex"; fileName = fName; buffer = new t_complex[bufferLength]; }
	TimeDiscreteAmplitudeContinuousComplex(string fName, int bLength) { type = "TimeDiscreteAmplitudeContinuousComplex"; fileName = fName; bufferLength = bLength; buffer = new t_complex[bLength]; }
	TimeDiscreteAmplitudeContinuousComplex(int bLength) { type = "TimeDiscreteAmplitudeContinuousComplex"; bufferLength = bLength; buffer = new t_complex[bLength]; }
	TimeDiscreteAmplitudeContinuousComplex(){ if (type == "") type = "TimeDiscreteAmplitudeContinuousComplex"; if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
};


class TimeContinuous : public Signal {
public:
	TimeContinuous(){}
};


class TimeContinuousAmplitudeDiscrete : public TimeContinuous {
public:
	TimeContinuousAmplitudeDiscrete(){}
};


class TimeContinuousAmplitudeContinuous : public TimeContinuous {
public:
	TimeContinuousAmplitudeContinuous(){}
};


class TimeContinuousAmplitudeDiscreteReal : public TimeContinuousAmplitudeDiscrete {
public:
	TimeContinuousAmplitudeDiscreteReal(string fName) { type = "TimeContinuousAmplitudeDiscreteReal";  fileName = fName; buffer = new t_real[bufferLength]; }
	TimeContinuousAmplitudeDiscreteReal(string fName, int bLength) { type = "TimeContinuousAmplitudeDiscreteReal";  fileName = fName; bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeDiscreteReal(int bLength) { type = "TimeContinuousAmplitudeDiscreteReal";  bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeDiscreteReal(){ if (type == "") type = "TimeContinuousAmplitudeDiscreteReal"; if (buffer == nullptr) buffer = new t_real[bufferLength]; }
};


class TimeContinuousAmplitudeDiscreteComplex : public TimeContinuousAmplitudeDiscrete {
public:
	TimeContinuousAmplitudeDiscreteComplex(string fName) { type = "TimeContinuousAmplitudeDiscreteComplex"; fileName = fName; buffer = new t_complex[bufferLength]; }
	TimeContinuousAmplitudeDiscreteComplex(string fName, int bLength) { type = "TimeContinuousAmplitudeDiscreteComplex"; fileName = fName; bufferLength = bLength; buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeDiscreteComplex(int bLength) { type = "TimeContinuousAmplitudeDiscreteComplex"; bufferLength = bLength; buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeDiscreteComplex(){ if (type == "") type = "TimeContinuousAmplitudeDiscreteComplex"; if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
};


class TimeContinuousAmplitudeContinuousReal : public TimeContinuousAmplitudeContinuous {
public:
	TimeContinuousAmplitudeContinuousReal(string fName) { type = "TimeContinuousAmplitudeContinuousReal"; fileName = fName; buffer = new t_real[bufferLength]; }
	TimeContinuousAmplitudeContinuousReal(string fName, int bLength) { type = "TimeContinuousAmplitudeContinuousReal"; fileName = fName; bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeContinuousReal(int bLength) { type = "TimeContinuousAmplitudeContinuousReal"; bufferLength = bLength; buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeContinuousReal(){ if (type == "") type = "TimeContinuousAmplitudeContinuousReal"; if (buffer == nullptr) buffer = new t_real[bufferLength]; }

	t_real bufferGet() {
		t_real value = static_cast<t_real *>(buffer)[outPosition];
		if (bufferFull) bufferFull = false;
		outPosition++;
		if (outPosition == bufferLength) outPosition = 0;
		if (outPosition == inPosition) bufferEmpty = true;
		return (value);
	};
};


class TimeContinuousAmplitudeContinuousComplex : public TimeContinuousAmplitudeContinuous {
public:
	TimeContinuousAmplitudeContinuousComplex(string fName) { type = "TimeContinuousAmplitudeContinuousComplex"; fileName = fName; buffer = new t_complex[bufferLength]; }
	TimeContinuousAmplitudeContinuousComplex(string fName, int bLength) { type = "TimeContinuousAmplitudeContinuousComplex"; fileName = fName; bufferLength = bLength; buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeContinuousComplex(int bLength) { type = "TimeContinuousAmplitudeContinuousComplex"; bufferLength = bLength; buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeContinuousComplex(){ if (type == "") type = "TimeContinuousAmplitudeContinuousComplex"; if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

	t_complex bufferGet() {
		t_complex value = static_cast<t_complex *>(buffer)[outPosition];
		if (bufferFull) bufferFull = false;
		outPosition++;
		if (outPosition == bufferLength) outPosition = 0;
		if (outPosition == inPosition) bufferEmpty = true;
		return (value);
	};


};


//########################################################################################################################################################
//########################################################## GENERIC BLOCKS DECLARATION AND DEFINITION ###################################################
//########################################################################################################################################################


// Descrives a generic Block.
class Block {
 public:
  	 
  void terminateBlock();
  virtual bool runBlock();
  
  int numberOfInputSignals;  
  int numberOfOutputSignals; 
  vector<Signal *> inputSignals;
  vector<Signal *> outputSignals;
};

class DiscreteToContinuousTime : public Block {
 public:
  DiscreteToContinuousTime(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
  bool runBlock(void);

  int numberOfSamplesPerSymbol{ 8 };
  int index { 0 };

  void setNumberOfSamplesPerSymbol(int nSamplesPerSymbol){ numberOfSamplesPerSymbol = nSamplesPerSymbol; outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol; };
};

// Generates a complex signal knowing the real part and the complex part.
class RealToComplex : public Block {
 public:
	 RealToComplex(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};

// Separates the complex signal into two parts: real and imaginary.
class ComplexToReal : public Block {
 public:
	 ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
  bool runBlock(void);
 //private:
};

class System {

 public:
  System(vector<Block *> &MainSystem);	
  void terminate();										
  void run();
  void run(string signalPath);

  string signalsFolder{ "signals" };
  char fileName[MAX_NAME_SIZE];  // Name of the file with system description (.sdf)
  char name[MAX_NAME_SIZE];  // System Name
  int numberOfBlocks;  // Number of system Blocks
  int (*topology)[MAX_TOPOLOGY_SIZE];  // Relationship matrix
  vector<Block *> SystemBlocks;  // Pointer to an array of pointers to Block objects
};

# endif // PROGRAM_INCLUDE_NETPLUS_H_


