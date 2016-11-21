
#include <InstanceCounter.h>

#include <iostream>
#include <string>

#include <Graph.h>


namespace
{
template<typename Type>
void printMemoryUsage(const std::string & name) {
    std::cout << name << '\n'
        << "  constructed: " << InstanceCounter<Type>::numAllocated << '\n'
        << "  destructed:  " << InstanceCounter<Type>::numDeallocated << '\n';
}
}


void printMemoryUsage()
{
    printMemoryUsage<Vertex>("Vertex");
    printMemoryUsage<Edge>("Edge");
    printMemoryUsage<Graph>("Graph");
}
