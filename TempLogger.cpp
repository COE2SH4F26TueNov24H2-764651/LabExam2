#include "TempLogger.hpp"

TempLogger::TempLogger() : readings(nullptr), count(0), offset(1.5) {}

TempLogger::TempLogger(int count, double initVal, double extraVal) : readings(nullptr), count(count), offset(extraVal) {
    if (count > 0) {
        readings = new double[count];
        for (int i = 0; i < count; ++i) {
            readings[i] = initVal + static_cast<double>(i);
        }
    } else {
        count = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
TempLogger::TempLogger(const TempLogger& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
TempLogger::~TempLogger() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
TempLogger& TempLogger::operator=(const TempLogger& other) {
    (void)other;
    return *this;
}

double TempLogger::getReading(int index) const {
    if (index >= 0 && index < count && readings != nullptr) {
        return readings[index];
    }
    return static_cast<double>(0);
}

// TODO 4: Implement averageReading() (Maximum 8 lines of implementation)
double TempLogger::averageReading() const {
    return static_cast<double>(0);
}
