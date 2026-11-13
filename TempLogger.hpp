#ifndef TEMPLOGGER_HPP
#define TEMPLOGGER_HPP

#include <iostream>
using namespace std;

class TempLogger {
protected:
    double* readings;
    int count;
    double offset;

public:
    TempLogger();
    TempLogger(int count, double initVal, double extraVal);
    TempLogger(const TempLogger& other);
    virtual ~TempLogger();
    TempLogger& operator=(const TempLogger& other);

    double getReading(int index) const;
    virtual double averageReading() const;
};

#endif
