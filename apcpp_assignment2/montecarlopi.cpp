#include <cstdlib>
#include <ctime>
#include <iostream>

#define TYPE double

using namespace std;

int main(int argc, char * argv[])
{
  srand(time(0));

  int inside {0};
  int outside {0};
  int break_counter {0};
  int iterations {0};
  clock_t b_time = clock();
  TYPE pi {0.0};

  do
  {
    TYPE x_val = ((TYPE) rand()) / RAND_MAX;
    TYPE y_val = ((TYPE) rand()) / RAND_MAX;
    
    if (x_val*x_val + y_val*y_val <= 1) {
      inside++;
    } else {
      outside++;
    }

    pi = ((TYPE) inside) / (inside + outside) * 4;

    if (pi >= 3.1413 && pi <= 3.1419)
    {
      break_counter++;
    }
    else 
    {
      break_counter = 0;
    }
    iterations++;
  } while (break_counter < 10000);

  clock_t e_time = clock();
  double elapsed = double(e_time - b_time) / CLOCKS_PER_SEC;

  cout << "After " << iterations-10000 << " iterations pi stayed within range for 10000 iterations. pi = " << pi << endl;

  return 0;
}
