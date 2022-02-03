#include "hazard-detection-unit.h"
#include "unistd.h"
#include <string>

bool HazardDetectionUnit::foundHazard()
{
    for(size_t i = 0; i < registers.size(); i++)
    {
        if(RS1 == registers[i] || RS2 == registers[i])
        {
            return true;
        }
    }
    RS1 = 0;
    RS2 = 0;
    return false;
}

void HazardDetectionUnit::setRS1(MemAddress num)
{
    RS1 = num;
}
void HazardDetectionUnit::setRS2(MemAddress num)
{
    RS2 = num;
}

void HazardDetectionUnit::push_back(MemAddress num)
{
    if (num != 0)
    {
        this->registers.push_back(num);
    }
}

void HazardDetectionUnit::erase(MemAddress num)
{
    if (registers.size() >= 1)
    {

        if (registers[0] == num)
        {
            
            registers.erase(registers.begin());
        }
    }
}