#ifndef __HAZARD_DETECTION_UNIT_H__
#define __HAZARD_DETECTION_UNIT_H__
#include <vector>
#include "arch.h"

class HazardDetectionUnit
{
public:
    HazardDetectionUnit() : RS1(0), RS2(0)
    {
    }
    bool foundHazard();
    void push_back(MemAddress num);
    void erase(MemAddress num);
    void setRS1(MemAddress num);
    void setRS2(MemAddress num);

    std::vector<MemAddress> registers{};

private:
    MemAddress RS1;
    MemAddress RS2;
};

#endif