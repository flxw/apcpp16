
#include <iostream>
#include <iomanip>

#include "ipc_viewer.h"


int main(int argc, char const * argv[])
{
    while (true)
    {
        const auto highScore = readHighScore();
        std::cout << "\n\n";
        for (const auto & item : highScore)
        {
            std::cout << std::setw(3) << item.first << ": "
                      << item.second << std::endl;
        }
    }
}
