
#include <iostream>

#include "provider.h"


int main(int argc, char * argv[])
{
    std::cout << provider::providerInfo(true) << std::endl;
    
    return 0;
}
