#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
  cout << "Hello ";

  switch(argc-1) {
    case 0: cout << "World"; break;
    case 1: cout << argv[1]; break;
    case 2: cout << argv[1] << " and " << argv[2]; break;
    default: for (int i=1; i<argc-1; ++i) { cout << argv[i] << ", "; }; cout << "and " << argv[argc-1]; break;
  }

  cout << endl;
  return 0;
}
