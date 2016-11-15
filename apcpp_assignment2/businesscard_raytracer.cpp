// usage: card > aek.ppm
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

struct Vector3D {
  float x;
  float y;
  float z;

  Vector3D() {}
  Vector3D(float a, float b, float c): x(a), y(b), z(c) {}

  Vector3D operator+(Vector3D r) // add
  {
    return Vector3D(x+r.x, y+r.y, z+r.z);
  }
  Vector3D operator*(float r) // scale
  {
    return Vector3D(x*r, y*r, z*r);
  }
  float operator%(Vector3D r) // dot product
  {
    return x*r.x + y*r.y + z*r.z;
  }
  Vector3D operator^(Vector3D r) // cross product
  {
    return Vector3D(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x);
  }
  Vector3D operator!() // normalize
  {
    return *this*(1/sqrt(*this%*this));
  }
};

// reuse the vector class to store colors
typedef Vector3D Color;

// the 3d scene ^^
// aek
// int G[] = {247570,280596,280600,249748,18578,18577,231184,16,16};
// felix
// int G[] = {34017857,34083362,34083348,63934984,34116116,34116130,34017345,37752832,25170432};
int G[9];




// void initScene(std::vector<std::string>& strScene, std::vector<int>& intScene) {
void initScene(std::vector<std::string>& strScene, int *intScene) {
  // for (auto& s : strScene) 
  for (int i = 0; i < 9; ++i)
  {
    // intScene.push_back(strtol(s.c_str(), NULL, 2));
    intScene[8-i] = strtol(strScene[i].c_str(), NULL, 2);
  }
}


float random01() { return ((float)rand())/RAND_MAX; }

enum TraceResult { sky, ground, sphere };

TraceResult trace(Vector3D origin, Vector3D direction, float& minDist, Vector3D& reflection)
{ 
  minDist = 1e9;
  TraceResult res = TraceResult::sky;
  float p = -origin.z/direction.z;
  if (.01 < p)
  {
    minDist = p;
    reflection = Vector3D(0,0,1);
    res = TraceResult::ground;
  }

  // iterate over columns of scene (there are 20)
  for (int column = 28; column >=0; column--)
  {
    // iterate over rows of scene (there are 9)
    for (int row = 8; row >= 0; row--)
    {
      // check if there is a sphere in the row/column
      if (G[row] & (1 << column))
      {        
        Vector3D p = origin + Vector3D(-column,0,-row-4);
        float b = p%direction;
        float c = p%p - 1;
        float q = b*b - c;
      
        // check if we hit the sphere
        if (q > 0)
        {
          // yes...
          float dist = -b - sqrt(q);
      
          // if this is a new minimum distance
          if (dist < minDist && dist > .01)
          {
            minDist = dist;
            reflection = !(p+direction*minDist);
            res = TraceResult::sphere;
          }
        }
      }
    }
  }

  return res;
}


Color sample(Vector3D origin, Vector3D direction)
{
  float intersectDist;
  Vector3D reflection;

  TraceResult traceRes = trace(origin, direction, intersectDist, reflection);

  if (traceRes == TraceResult::sky)
  {
    // get a magic sky color 
    return Color(.7, .6, 1)*pow(1-direction.z, 4);
  }

  Vector3D intersectPoint = origin + direction*intersectDist;
  // what are those?
  Vector3D l = !(Vector3D(9 + random01(), 9 + random01(), 16) + intersectPoint*-1);
  Vector3D r = direction + reflection*((reflection%direction)*-2);

  // what is that?
  float b = l%reflection;

  if (b < 0 || TraceResult::sky != trace(intersectPoint, l, intersectDist, reflection))
  {
    b = 0;
  }

  float p = pow((l%r)*(b>0 ? 1 : 0), 99);

  if (traceRes == TraceResult::ground)
  {
    intersectPoint = intersectPoint*.2;
    Color accentSquare(3, 1, 1);
    Color mainSquare(3, 3, 3);

    return (((int)(ceil(intersectPoint.x) + ceil(intersectPoint.y))) % 2 == 0 ? mainSquare : accentSquare)*(b*.2 + .1);
  }

  // we hit a sphere
  return Color(p, p, p) + sample(intersectPoint, r)*.5;
}

int main()
{
  std::vector<std::string> scene = {
    "01100000000001001000000000",
    "10010000000001000000000000",
    "10000001110001000001000001",
    "10000010001001001000100010",
    "10000010001001001000010100",
    "11110011111001001000001000",
    "10000010000001001000010100",
    "10000010000001001000100010",
    "10000001110001001001000001"
  };

  initScene(scene, G);

  // ppm header
  std::cout << "P6 512 512 255 "; 

  Vector3D camPosition = Vector3D(17, 32, 12);
  Vector3D camDirection = !Vector3D(-2, -16, 0);
  Vector3D a = !(Vector3D(0,0,1)^camDirection)*.002;
  Vector3D b = !(camDirection^a)*.002;
  Vector3D c = (a + b)*-256 + camDirection;

  for (int y = 512; y--;)
  {
    for (int x = 512; x--;)
    {
      Color p(13, 13, 13);

      // trace 64 rays
      for (int r = 64; r--;)
      {       
        // some random delta
        Vector3D t = a*(random01() - .5)*99 + b*(random01() - .5)*99;
        // add up the color value for this pixel
        p = p + sample(camPosition + t, !(t*-1 + (a*(random01() + x) + b*(y + random01()) + c)*16))*3.5;
      }
      // can we do something about this casting mess?
      std::cout << (char)(int)p.x << (char)(int)p.y << (char)(int)p.z; 
    }
  }
}