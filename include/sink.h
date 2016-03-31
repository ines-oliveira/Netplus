# ifndef SINK_H_
# define SINK_H_

# include "netplus.h"

class Sink : public Block {

public:
	Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	long int numberOfSamples{ -1 };

	void setNumberOfSamples(long int nOfSamples){ numberOfSamples = nOfSamples; };

};

#endif