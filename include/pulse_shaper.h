# ifndef PULSE_SHAPER_H_
# define PULSE_SHAPER_H_

# include <vector>
# include "netplus.h"

using namespace std;

/* Raised-cosine filter with an impulsional response of a number of taps
equal to "numberOfTaps" and roll-off factor equal to "rollOff". */
class PulseShaper : public Block {

	double rollOffFactor{ 0.3 };// Roll-off factor (roll)
	int numberOfTaps{ 4 };  //in units of symbol period

public:
	PulseShaper(vector<Signal *> &InputSig, vector<Signal *> OutputSig);

	bool runBlock(void);

	int impulseResponseLength;

	vector<t_real> impulseResponse;
	string impulseResponseFilename{ "pulse_shapper_impulse_response.imp" };

	vector<t_real> response;

	void setRollOffFactor(double rollOff){ rollOffFactor = rollOff; }

	void setNumberOfTaps(int nTaps){ numberOfTaps = nTaps; }
};

# endif


