#ifndef OFFSETLOGGER_HPP
#define OFFSETLOGGER_HPP

#include "TempLogger.hpp"

class OffsetLogger : public TempLogger {
public:
    OffsetLogger();
    OffsetLogger(int count, double initVal, double extraVal);
    OffsetLogger(const OffsetLogger& other);
    virtual ~OffsetLogger();
    OffsetLogger& operator=(const OffsetLogger& other);

    virtual double averageReading() const override;
};

#endif
