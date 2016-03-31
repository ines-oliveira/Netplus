# ifndef NETPLUS_LIB_H_
# define NETPLUS_LIB_H_

# include <vector>
# include "netplus.h"

using namespace std;

/* Realizes the M-QAM mapping. */
class MQamMapper : public Block {
public:
	MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	t_real amplitude {1};
	t_binary * reg = new t_binary[2];
	int inReg{ 0 };


};

/* Raised-cosine filter with an impulsional response of a number of taps
equal to "numberOfTaps" and roll-off factor equal to "rollOff". */
class PulseShaper : public Block {

public:
	PulseShaper(vector<Signal *> &InputSig, vector<Signal *> OutputSig);
	
	bool runBlock(void);
	
	double rollOffFactor{ 0.9 };// Roll-off factor (roll)
	
	int impulseResponseTimeLength{4};  //in units of symbol period

	int impulseResponseLength;

	vector<t_real> impulseResponse;
	string impulseResponseFilename{ "pulse_shapper_impulse_response.imp" };

	vector<t_real> response;
	
	void setRollOffFactor(double rOffFactor){ rollOffFactor = rOffFactor; };
};

# endif


