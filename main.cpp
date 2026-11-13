#include <iostream>
#include "TempLogger.hpp"
#include "OffsetLogger.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    double initV = static_cast<double>(20.0);
    double extV = static_cast<double>(1.5);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        TempLogger pDef;
        if (approxEqual(static_cast<double>(pDef.averageReading()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getReading(0)), 0.0)) {
            cout << "[PASS] TC 01: TempLogger default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: TempLogger default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        TempLogger p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getReading(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getReading(2)), static_cast<double>(initV + static_cast<double>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getReading(1) == static_cast<double>(initV + static_cast<double>(1))) {
            // Compare against a second identical instance
            TempLogger pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.averageReading()), static_cast<double>(pCheck.averageReading())) &&
                !approxEqual(static_cast<double>(p1.averageReading()), 0.0)) {
                cout << "[PASS] TC 02: TempLogger parameterized constructor & averageReading()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: TempLogger averageReading() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: TempLogger element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        TempLogger orig(3, initV, extV);
        TempLogger copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.averageReading()), static_cast<double>(orig.averageReading())) &&
            approxEqual(static_cast<double>(copyObj.getReading(1)), static_cast<double>(orig.getReading(1))) &&
            !approxEqual(static_cast<double>(copyObj.averageReading()), 0.0)) {
            cout << "[PASS] TC 03: TempLogger copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: TempLogger copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        TempLogger a(3, initV, extV);
        TempLogger b(2, initV, extV);
        b = a;
        TempLogger* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.averageReading()), static_cast<double>(a.averageReading())) &&
            approxEqual(static_cast<double>(b.getReading(2)), static_cast<double>(a.getReading(2))) &&
            !approxEqual(static_cast<double>(b.averageReading()), 0.0)) {
            cout << "[PASS] TC 04: TempLogger copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: TempLogger copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            TempLogger temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: TempLogger destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: TempLogger destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        TempLogger baseObj(3, initV, extV);
        OffsetLogger childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.averageReading()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.averageReading()), static_cast<double>(baseObj.averageReading()))) {
            cout << "[PASS] TC 06: OffsetLogger overridden averageReading()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: OffsetLogger overridden averageReading() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        OffsetLogger directChild(3, initV, extV);
        TempLogger* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->averageReading()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->averageReading()), static_cast<double>(directChild.averageReading()))) {
            cout << "[PASS] TC 07: OffsetLogger polymorphic dispatch via TempLogger*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: OffsetLogger polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        OffsetLogger c1(3, initV, extV);
        OffsetLogger c2(c1);
        if (approxEqual(static_cast<double>(c1.averageReading()), static_cast<double>(c2.averageReading())) &&
            !approxEqual(static_cast<double>(c2.averageReading()), 0.0)) {
            cout << "[PASS] TC 08: OffsetLogger copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: OffsetLogger copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        OffsetLogger c1(3, initV, extV);
        OffsetLogger c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.averageReading()), static_cast<double>(c2.averageReading())) &&
            !approxEqual(static_cast<double>(c2.averageReading()), 0.0)) {
            cout << "[PASS] TC 09: OffsetLogger copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: OffsetLogger copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        TempLogger* heapChild = new OffsetLogger(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: OffsetLogger virtual destructor via TempLogger*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: OffsetLogger virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
