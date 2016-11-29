#include "particle.h"

#include <algorithm>
#include <iostream>

using namespace std;

// ToDo: define memory managment here

const unsigned long blockSize = 1.5*1024*1024;
const unsigned long particleCount = blockSize / sizeof(Particle);
Particle* particleMemoryBlock = (Particle*)malloc(blockSize);
int occupationArray[particleCount];
bool initialized = false;

int getNextFreeBlock() {
  static unsigned long i = 0;

  for (; i < particleCount; ++i) {
    if (occupationArray[i] == -1) return i;
  }

  i = 0;

  return -1; // everything is full
}

static void* Particle::operator new(size_t sz) throw() {
  // the occupationArray may still not be initialized
  if (!initialized) {
    for (unsigned long k = 0; k < particleCount; ++k) {
      occupationArray[k] = -1;
    }
    initialized = true;
  }

  unsigned long nextFreeBlock = getNextFreeBlock();

  if (nextFreeBlock == -1) {
      return nullptr;
  }

  occupationArray[nextFreeBlock] = 1;

  return (void*) &particleMemoryBlock[nextFreeBlock];
}

static void Particle::operator delete(void* ptr) {
  int offset = (int) ((Particle *)ptr - particleMemoryBlock);
  occupationArray[offset] = -1;
}
