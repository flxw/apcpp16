#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
  srand(time(0));

  uint64_t inside {0};
  uint64_t outside {0};
  uint64_t iterations {0};
  uint64_t pi_int {0};

  uint32_t break_counter {0};
  
  clock_t b_time = clock();

  do
  {
    double x_val = double(rand()) / RAND_MAX;
    double y_val = double(rand()) / RAND_MAX;
    
    if (x_val*x_val + y_val*y_val <= 1) {
      inside++;
    } else {
      outside++;
    }

    // to avoid float type for pi, omit division by (inside + outside),
    // this is neutralized in the next line
    pi_int = inside * 4;

    // multiply with 10000 to avoid conversion to float
    if (pi_int * 10000 >= 31413 * (inside + outside) && pi_int * 10000 <= 31419 * (inside + outside))
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

  double pi = double(pi_int) / (inside + outside);

  cout << "After " << iterations-10000 << " iterations pi stayed within range for 10000 iterations. pi = " << pi << endl;

  return 0;
}
