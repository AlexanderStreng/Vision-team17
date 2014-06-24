#ifndef _BASETIMER_H_
#define _BASETIMER_H_

#include <string>
#include <fstream>
#include <map>

typedef std::map<std::string, unsigned long long> valuelist;
class BaseTimer {
private:
	valuelist __values;

	unsigned long long GetPerformanceTicks();
	unsigned long long GetPerformanceTicksInSecond();
protected:
	unsigned long long  _start;
	unsigned long long  _stop;
	unsigned long long _nFreq;
public:
	BaseTimer();

	bool start();
	bool stop();
	void reset();

	unsigned long long elapsedMicroSeconds();
	unsigned long long elapsedMilliSeconds();
	double             elapsedSeconds();

	void store(std::string label);
	bool save(std::string filename);
};

#endif  // _BASETIMER_H_