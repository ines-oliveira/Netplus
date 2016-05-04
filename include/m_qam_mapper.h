# ifndef M_QAM_MAPPER_H_
# define M_QAM_MAPPER_H_

# include <vector>
# include "netplus.h"

using namespace std;

/* Realizes the M-QAM mapping. */
class MQamMapper : public Block {
public:
	MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	t_real amplitude{ sqrt(2) / 2 };
	t_binary * reg = new t_binary[2];
	int inReg{ 0 };


};

#endif