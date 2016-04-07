# include "..\include\netplus.h"
# include "..\include\sink.h"

#include <algorithm>    // std::min

using namespace std;

Sink::Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
}

bool Sink::runBlock(void)
{

	int ready = inputSignals[0]->ready();

	int process;
	if (numberOfSamples >= 0) {
		process = std::min((long int)ready, numberOfSamples);
	}
	else {
		process = ready;
	}
	 
	if ((process == 0) || (numberOfSamples==0)) return false;

	for (int i = 0; i<process; i++) {
		t_complex myComplex = static_cast<TimeContinuousAmplitudeContinuousComplex *>(inputSignals[0])->bufferGet();
		if (numberOfSamples > 0) numberOfSamples--;
	}
	
	cout << numberOfSamples << "\n";

	return true;
}
