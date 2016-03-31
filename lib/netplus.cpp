# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <algorithm>

# include "netplus.h"


using namespace std;

//########################################################################################################################################################
//######################################################### SIGNALS FUNCTIONS IMPLEMENTATION #############################################################
//########################################################################################################################################################




void Signal::writeHeader(){

	string signalPath{ "signals" };

	ofstream headerFile;

	if (!fileName.empty()) {

		headerFile.open("./" + signalPath + "/" + fileName, ios::out);

		headerFile << "Signal type: " << type << "\n";
		headerFile << "Symbol Period (s): " << symbolPeriod << "\n";
		headerFile << "Sampling Period (s): " << samplingPeriod << "\n";

		headerFile << "// ### HEADER TERMINATOR ###\n";

		headerFile.close();
	}


};

void Signal::writeHeader(string signalPath){

	ofstream headerFile;

	if (!fileName.empty()) {

		headerFile.open("./" + signalPath + "/" + fileName, ios::out);

		headerFile << "Signal type: " << type << "\n";
		headerFile << "Symbol Period (s): " << symbolPeriod << "\n";
		headerFile << "Sampling Period (s): " << samplingPeriod << "\n";

		headerFile << "// ### HEADER TERMINATOR ###\n";

		headerFile.close();
	}


};



//########################################################################################################################################################
//###################################################### GENERAL BLOCKS FUNCTIONS IMPLEMENTATION #########################################################
//########################################################################################################################################################



void Block::terminateBlock(void) {

  for (int i = 0; i < numberOfInputSignals; i++)
    inputSignals[i]->close();

}

bool Block::runBlock(void) {
	return false;
}


DiscreteToContinuousTime::DiscreteToContinuousTime(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = (inputSignals[0]->symbolPeriod);
  outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol;

  
}

bool DiscreteToContinuousTime::runBlock(void) {

	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	if (index != 0) {
		for (int i = index; (i < numberOfSamplesPerSymbol) & (space>0); i++) {
			outputSignals[0]->bufferPut(0);
			alive = true;
			space--;
			index++;
		};
		if (index == numberOfSamplesPerSymbol) index = 0;
	};

	int length = min((int)ceil((double) space / (double)numberOfSamplesPerSymbol), ready);

	if (length <= 0) return alive;

	for (int i = 0; i < length; i++) {
		t_real value = (t_real)(static_cast<TimeDiscreteAmplitudeDiscreteReal *>(inputSignals[0]))->bufferGet();
		outputSignals[0]->bufferPut(value);
		space--;
		index++;
		for (int k = 1; (k<numberOfSamplesPerSymbol) & (space>0); k++) {
			outputSignals[0]->bufferPut((t_real) 0.0);
			space--;
			index++;
		}
		if (index == numberOfSamplesPerSymbol) index = 0;
	}

	return true;

}


RealToComplex::RealToComplex(vector <Signal *> &InputSig, vector <Signal *> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
  outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
}

bool RealToComplex::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	for (int i = 0; i < process; i++) {

		t_real re = static_cast<TimeContinuousAmplitudeContinuousReal *>(inputSignals[0])->bufferGet();
		t_real im = static_cast<TimeContinuousAmplitudeContinuousReal *>(inputSignals[1])->bufferGet();

		complex<t_real> myComplex( re, im);

		outputSignals[0]->bufferPut(myComplex);

	}

	return true;
}
//
//ComplexToReal::ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {
//
//  numberOfInputSignals = 1;
//  numberOfOutputSignals = 2;
//  InputSignals = InputSig;
//  OutputSignals = OutputSig;
//
//  outputSignals[0]->period = 2*InputSignals[0]->period;
//  outputSignals[1]->period = 2*InputSignals[0]->period;
//  static_cast<TimeContinuousAmplitudeContinuousReal*>(outputSignals[0])->samplingTime =
//  2*static_cast<TimeContinuousAmplitudeContinuousComplex*>(InputSignals[0])->samplingTime;
//  static_cast<TimeContinuousAmplitudeContinuousReal*>(outputSignals[1])->samplingTime =
//  2 * static_cast<TimeContinuousAmplitudeContinuousComplex*>(InputSignals[0])->samplingTime;
//
//}
//
//bool ComplexToReal::runBlock(void){
//
//  FILE *file = fopen(outputSignals[0]->fileName, "a");
//  FILE *file2 = fopen(outputSignals[1]->fileName, "a");
//
//  int length_in = InputSignals[0]->bufferLength;
//  int length_out1 = outputSignals[0]->bufferLength;
//  int length_out2 = outputSignals[1]->bufferLength;
//
//  double *auxil1 = static_cast<double*>(outputSignals[0]->buffer);
//  double *auxil2 = static_cast<double*>(outputSignals[1]->buffer);
//
//  if (InputSignals[0]->outPosition >= length_in ||
//      outputSignals[0]->inPosition >= length_out1 ||
//      outputSignals[1]->inPosition >= length_out2) {
//
//    fclose(file);
//    fclose(file2);
//    return false;
//
//  } else {
//    for (int i = InputSignals[0]->outPosition; i < length_in; i ++) {
//      if (outputSignals[0]->inPosition < length_out1 &&
//          outputSignals[1]->inPosition < length_out2) {
//
//        auxil1[i] = static_cast<complex<double>*>(InputSignals[0]->buffer)[i].real();
//        auxil2[i] = static_cast<complex<double>*>(InputSignals[0]->buffer)[i].imag();
//
//        InputSignals[0]->outPosition++;
//        outputSignals[0]->inPosition++;
//        outputSignals[1]->inPosition++;
//        outputSignals[0]->count++;
//        outputSignals[1]->count++;
//
//        // File 1
//        if ((outputSignals[0]->count >= outputSignals[0]->firstValueToBeSaved) &&
//            (outputSignals[0]->numberOfSavedValues <
//             outputSignals[0]->numberOfValuesToBeSaved)) {
//
//          fprintf(file, "%7.4f,",
//                 (static_cast<double*>(outputSignals[0]->buffer))[outputSignals[0]->inPosition - 1]);
//
//          outputSignals[0]->numberOfSavedValues++;
//
//          if (outputSignals[0]->numberOfSavedValues ==
//              outputSignals[0]->numberOfValuesToBeSaved)
//            fprintf(file, "\0");
//        }
//
//        // File 2
//        if ((outputSignals[1]->count >= outputSignals[1]->firstValueToBeSaved) &&
//            (outputSignals[1]->numberOfSavedValues <
//             outputSignals[1]->numberOfValuesToBeSaved)) {
//
//          fprintf(file2, "%7.4f,",
//                 (static_cast<double*>(outputSignals[1]->buffer))[outputSignals[1]->inPosition - 1]);
//
//          outputSignals[1]->numberOfSavedValues++;
//
//          if (outputSignals[1]->numberOfSavedValues ==
//              outputSignals[1]->numberOfValuesToBeSaved)
//            fprintf(file2, "\0");
//        }
//
//      } else {
//        break;
//      }
//    }
//
//    outputSignals[0]->outPosition = 0;
//    outputSignals[1]->outPosition = 0;
//
//    if (InputSignals[0]->outPosition == length_in) {
//      InputSignals[0]->inPosition = 0;
//      fclose(file);
//      fclose(file2);
//      return true;
//    } else {
//      fclose(file);
//      fclose(file2);
//      return false;
//    }
//  }
//
//}
//
//ContinuousToDiscreteTime::ContinuousToDiscreteTime(vector<Signal *> &InputSig, vector<Signal *> &OutputSig, double time){
//
//  numberOfInputSignals = 1;
//  numberOfOutputSignals = 1;
//  InputSignals = InputSig;
//  OutputSignals = OutputSig;
//
//  timeSampling = time;
//  outputSignals[0]->period = timeSampling;
//
//  static_cast<TimeDiscreteAmplitudeContinuousReal*>(outputSignals[0])->samplingPeriod = timeSampling;
//
//}
//
//bool ContinuousToDiscreteTime::runBlock(void){
//
//  FILE *file = fopen(outputSignals[0]->fileName, "a");
//
//  int length_in = InputSignals[0]->bufferLength;
//  int length_out = outputSignals[0]->bufferLength;
//  double nS = timeSampling / 
//              static_cast<TimeContinuousAmplitudeContinuousReal*>(InputSignals[0])->samplingTime;
//
//  int nSamples = static_cast<int>(nS);
//
//  double *auxil = static_cast<double*>(outputSignals[0]->buffer);
//
//  if (InputSignals[0]->outPosition >= length_in || outputSignals[0]->inPosition >= length_out) {
//    fclose(file);
//    return false;
//  } else {
//
//    for (int i = InputSignals[0]->outPosition; i < length_in; i += nSamples) {
//
//      if (outputSignals[0]->inPosition < length_out) {
//
//        auxil[i/nSamples] = (static_cast<double*>(InputSignals[0]->buffer))[i];
//
//        outputSignals[0]->inPosition++;
//        outputSignals[0]->count++;
//
//        // File 
//        if ((outputSignals[0]->count >= outputSignals[0]->firstValueToBeSaved) &&
//            (outputSignals[0]->numberOfSavedValues <
//             outputSignals[0]->numberOfValuesToBeSaved)) {
//
//            fprintf(file, "%6.3f,",
//            (static_cast<double*>(outputSignals[0]->buffer))[outputSignals[0]->inPosition - 1]);
//
//            outputSignals[0]->numberOfSavedValues++;
//
//            if (outputSignals[0]->numberOfSavedValues ==
//                outputSignals[0]->numberOfValuesToBeSaved)
//              fprintf(file, "\0");
//         }
//
//         InputSignals[0]->outPosition += nSamples;
//
//       } else {
//        break;
//      }
//    }
//
//    outputSignals[0]->outPosition = 0;
//
//    if (InputSignals[0]->outPosition == length_in) {
//      InputSignals[0]->inPosition = 0;
//      fclose(file);
//      return true;
//    } else {
//      fclose(file);
//      return false;
//    }
//  }
//
//}
//



System::System(vector<Block *> &Blocks) {

	SystemBlocks = Blocks;

}

void System::run() {

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		for (int unsigned j = 0; j<(SystemBlocks[i]->inputSignals).size(); j++) {
			(SystemBlocks[i]->inputSignals[j])->writeHeader();
		}
	}

	bool Alive;
	do {
		Alive = false;
		for (unsigned int i = 0; i < SystemBlocks.size(); i++) {
			bool aux = SystemBlocks[i]->runBlock();
			Alive = (Alive || aux);
		}
	} while (Alive);

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->terminateBlock();
	}
}

void System::run(string signalPath) {

	// The signalPath sub-folder must already exists

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		for (int unsigned j = 0; j<(SystemBlocks[i]->inputSignals).size(); j++) {
			(SystemBlocks[i]->inputSignals[j])->writeHeader(signalPath);
		}
	}

	bool Alive;
	do {
		Alive = false;
		for (unsigned int i = 0; i < SystemBlocks.size(); i++) {
			Alive = Alive || SystemBlocks[i]->runBlock();
		}
	} while (Alive);

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->terminateBlock();
	}
}

