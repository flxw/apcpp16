
#include <string>

#include "externallibrary.h"

int main(int argc, char * argv[])
{
    
    bool dryRun = atoi(argv[1]) < 1000000;

    initialize(dryRun);

    while (running())
    {
        processEvents();
    }

    return 0;
}
