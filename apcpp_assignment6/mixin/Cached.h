#include <vector>
#include <utility>

#include "BezierCurve.h"
#include "Circle.h"

using namespace std;

// GENERALIZATION
template<typename T> class Cached : public T {
    void setCacheResolution(const unsigned int& res) { cacheResolution = res; /*calculateCache();*/ }
    void setUseLinearInterpolation(const bool& b) { useInterpolation = b; }

    pair<float,float> operator() (float t) const {
      int pos = std::lround(t/(1/(float)cacheResolution));
      return cache[pos];
    }

  private:
    bool useInterpolation;
    unsigned int cacheResolution;
    vector<pair<float,float>> cache;
    void calculateCache() {
      cache.clear();
      for (auto t = 0.0f; t <= 1.0f; t += 1.0f / (float)cacheResolution)
      {
        cache.push_back(this->Circle(t));
      }
    }
};
