# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm> 
# include <random>

# include "..\include\netplus.h"
# include "..\include\binary_source.h"

# define LOGFILE "gl.log"     // all Log(); messages will be appended to this file

using namespace std;

BinarySource::BinarySource(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = bitPeriod;
  outputSignals[0]->samplingPeriod = outputSignals[0]->symbolPeriod;

}

bool BinarySource::runBlock(void) {

	int space = outputSignals[0]->space();

	int process;
	if (numberOfBits >= 0) {
		process = std::min((long int) space, numberOfBits);
	}
	else {
		process = space;
	}

	if (process <= 0) return false;

	if (mode == PseudoRandom){

		int len = patternLength;

		if (acumul.size() == 0){
			acumul.resize(len + 1);
			for (int i = 0; i < len; i++)	acumul[i] = 0;
			acumul[len - 1] = 1;
		}

		if (len <= 8){						// constraint
			if (!bitStream.empty()){
				std::vector<char> values(bitStream.begin(), bitStream.end());
				if (validBitStream(len, values)){
					for (int i = 0; i < len; i++)
						acumul[len - i - 1] = values[i] - '0';
				}
			}
		}

		vector<int>& ac = acumul;

		for (int k = 0; k < process; k++) {

			outputSignals[0]->bufferPut((t_binary)ac[len]);
			numberOfBits--;

			for (int i = len; i > 0; --i) ac[i] = ac[i - 1];

			switch (len) {
			case 1:
				ac[0] = ac[1];
				break;
			case 2:
				ac[0] = (ac[1] + ac[2]) % 2;
				break;
			case 3:
				ac[0] = (ac[1] + ac[3]) % 2;
				break;
			case 4:
				ac[0] = (ac[1] + ac[4]) % 2;
				break;
			case 5:
				ac[0] = (ac[1] + ac[2] + ac[4] + ac[5]) % 2;
				break;
			case 6:
				ac[0] = (ac[2] + ac[3] + ac[5] + ac[6]) % 2;
				break;
			case 7:
				ac[0] = (ac[1] + ac[7]) % 2;
				break;
			case 8:
				ac[0] = (ac[2] + ac[3] + ac[4] + ac[8]) % 2;
				break;
			case 9:
				ac[0] = (ac[3] + ac[4] + ac[6] + ac[9]) % 2;
				break;
			case 10:
				ac[0] = (ac[3] + ac[10]) % 2;
				break;
			case 11:
				ac[0] = (ac[11] + ac[8] + ac[5] + ac[2]) % 2;
				break;
			case 12:
				ac[0] = (ac[1] + ac[4] + ac[6] + ac[12]) % 2;
				break;
			case 13:
				ac[0] = (ac[1] + ac[3] + ac[4] + ac[13]) % 2;
				break;
			case 14:
				ac[0] = (ac[2] + ac[12] + ac[13] + ac[14]) % 2;
				break;
			case 15:
				ac[0] = (ac[1] + ac[15]) % 2;
				break;
			case 16:
				ac[0] = (ac[2] + ac[3] + ac[5] + ac[16]) % 2;
				break;
			case 17:
				ac[0] = (ac[14] + ac[17]) % 2;
				break;
			case 18:
				ac[0] = (ac[1] + ac[2] + ac[5] + ac[18]) % 2;
				break;
			case 19:
				ac[0] = (ac[1] + ac[2] + ac[5] + ac[19]) % 2;
				break;
			case 20:
				ac[0] = (ac[8] + ac[9] + ac[17] + ac[20]) % 2;
				break;
			case 21:
				ac[0] = (ac[19] + ac[21]) % 2;
				break;
			case 22:
				ac[0] = (ac[1] + ac[22]) % 2;
				break;
			case 23:
				ac[0] = (ac[5] + ac[23]) % 2;
				break;
			case 24:
				ac[0] = (ac[1] + ac[3] + ac[4] + ac[24]) % 2;
				break;
			case 25:
				ac[0] = (ac[3] + ac[25]) % 2;
				break;
			case 26:
				ac[0] = (ac[8] + ac[24] + ac[25] + ac[26]) % 2;
				break;
			case 27:
				ac[0] = (ac[1] + ac[2] + ac[5] + ac[27]) % 2;  //not verified
				break;
			case 28:
				ac[0] = (ac[3] + ac[28]) % 2;  //not verified
				break;
			case 29:
				ac[0] = (ac[2] + ac[29]) % 2;  //not verified
				break;
			case 30:
				ac[0] = (ac[2] + ac[4] + ac[6] + ac[30]) % 2;  //not verified
				break;
			case 31:
				ac[0] = (ac[3] + ac[31]) % 2;  //not verified
				break;
			case 32:
				ac[0] = (ac[1] + ac[2] + ac[3] + ac[5] + ac[7] + ac[32]) % 2; //not verified
				break;
			}
		}
	}

	if (mode == Random){

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 1);

		t_binary value;
		for (int k = 0; k < process; k++) {
			value = dis(gen);
			outputSignals[0]->bufferPut((t_binary) value);
			numberOfBits--;
		}
	}

	if (mode == Deterministic){
		std::vector<char> values(bitStream.begin(), bitStream.end());
		int valuesSize = values.size();
		for (int k = 0; k < process; k++) {
			outputSignals[0]->bufferPut((t_binary)(values[posBitStream++] - '0'));
			numberOfBits--;
			posBitStream = posBitStream % valuesSize;
		}

	}

	return true;
}

void BinarySource :: setBitPeriod(double bPeriod){
	bitPeriod = bPeriod;
	outputSignals[0]->symbolPeriod = bitPeriod;
	outputSignals[0]->samplingPeriod = outputSignals[0]->symbolPeriod;
};

bool BinarySource::validBitStream(int pLength, vector<char>& values){
	patternLength = pLength;

	int valuesSize = values.size();

	if (valuesSize != pLength) {
		if (valuesSize > pLength)
			error(1);
		else
			error(2);
		values.resize(pLength + 1);
		for (int j = valuesSize; j < pLength + 1; j++)
			values[j] = '0';
	}

	bool one = false;
	for (int i = 0; i < pLength; i++){
		if (values[i] != '0' && values[i] != '1') {
			error(3);
			return false;
		}
		if (values[i] == '1') one = true;
	}
	if (!one) error(4);

	return one;
};

void BinarySource::error(int errorType){
	string mError;

	switch (errorType){
	case 1:
		mError = "WARNING: Bit string length greater than pattern length! Last bit values are discarded.";
		break;
	case 2:
		mError = "WARNING: Bit string length smaller than pattern length! ZERO bit value is added at the end.";
		break;
	case 3:
		mError = "WARNING: Bit values must be ZERO or ONE! Bit values changed to '100...'";
		break;
	case 4:
		mError = "WARNING: At least one bit must have the value ONE! Bit values changed to '100...'";
		break;
	}

	std::vector<char> message(mError.begin(), mError.end());
	Log(&*message.begin());
	Log("\n");
};


extern bool LogCreated = false;

void BinarySource::Log(char *message)
{
	FILE *file;

	if (!LogCreated) {
		file = fopen(LOGFILE, "w");
		LogCreated = true;
	}
	else
		file = fopen(LOGFILE, "r");

	if (file == NULL) {
		if (LogCreated)
			LogCreated = false;
		return;
	}
	else
	{
		fputs(message, file);
		fclose(file);
	}

	if (file)
		fclose(file);
};