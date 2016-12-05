
#include <iostream>


const auto pi = 3.1415926535897932384626433f;


float fastSin(float x)
{
	return 0.0f;
}


int main(int argc, char const *argv[])
{
    size_t resolution = 1024;
    for (size_t i = 0; i <= resolution; ++i)
    {
        float actual = fastSin(i * 2.0f * pi / resolution);
        float expected = std::sin(i * 2.0f * pi / resolution);
        
        std::cout << actual << "/" << expected << " -> " << std::abs(actual - expected) << std::endl;
    }
}
