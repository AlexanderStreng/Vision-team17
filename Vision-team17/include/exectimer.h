#ifndef _EXECTIMER_H_
#define _EXECTIMER_H_

#include "basetimer.h"
#include <string>

class ExecTimer : public BaseTimer {
public:
	ExecTimer();
	bool run(std::string cmd);
};

#endif // _EXECTIMER_H_