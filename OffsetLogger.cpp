#include "OffsetLogger.hpp"

OffsetLogger::OffsetLogger() : TempLogger() {}

OffsetLogger::OffsetLogger(int count, double initVal, double extraVal) : TempLogger(count, initVal, extraVal) {}

OffsetLogger::OffsetLogger(const OffsetLogger& other) : TempLogger(other) {}

OffsetLogger::~OffsetLogger() {}

OffsetLogger& OffsetLogger::operator=(const OffsetLogger& other) {
    if (this != &other) {
        TempLogger::operator=(other);
    }
    return *this;
}

// TODO 5: Override averageReading() (Maximum 8 lines of implementation)
double OffsetLogger::averageReading() const {
    return static_cast<double>(0);
}
