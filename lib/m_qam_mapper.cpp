# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm>

# include "netplus.h"
# include "m_qam_mapper.h"


using namespace std;

MQamMapper::MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {
     
  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;
 
  outputSignals[0]->symbolPeriod = 2 * (inputSignals[0]->symbolPeriod);
  outputSignals[0]->samplingPeriod = 2 * inputSignals[0]->samplingPeriod;

  outputSignals[1]->symbolPeriod = 2 * (inputSignals[0]->symbolPeriod);
  outputSignals[1]->samplingPeriod = 2 * inputSignals[0]->samplingPeriod;

}

bool MQamMapper::runBlock(void) {


	int ready = inputSignals[0]->ready();
	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[1]->space();
	int space = (space1 <= space2) ? space1 : space2;
	int length = min(ready, 2*space);

	if (length <= 0) return false;

	for (int i = 0; i < length; i++) {
		reg[inReg] = static_cast<Binary *>(inputSignals[0])->bufferGet();
		inReg++;
		if (inReg == 2) {
			if (reg[0] == 0)
				outputSignals[0]->bufferPut(amplitude);
			else
				outputSignals[0]->bufferPut(-amplitude);
			reg[1] == 0 ? outputSignals[1]->bufferPut(amplitude) : outputSignals[1]->bufferPut(-amplitude);
			inReg = 0;
		}
	}

	return true;
}

