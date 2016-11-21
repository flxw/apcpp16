
#pragma once

#include <cstdlib>


void printMemoryUsage();


template<typename Type>
struct InstanceCounter {
    static std::size_t numAllocated;
    static std::size_t numDeallocated;
protected:
    InstanceCounter() { ++numAllocated; }
    ~InstanceCounter() { ++numDeallocated; }
    friend Type;
};
template<typename Type>
std::size_t InstanceCounter<Type>::numAllocated = 0;
template<typename Type>
std::size_t InstanceCounter<Type>::numDeallocated = 0;
