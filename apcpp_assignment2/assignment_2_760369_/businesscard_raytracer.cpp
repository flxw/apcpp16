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


class Raytracer 
{
public:
  Raytracer(std::vector<std::string>& strScene) 
  {
    this->initScene(strScene);
  }

  void setCamPosition(Vector3D& pos)
  {
    camPosition = pos;
  }

  void setCamDirection(Vector3D& dir)
  {
    camDirection = dir;
  }

  void run()
  {
    // ppm header
    std::cout << "P6 512 512 255 "; 

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
          Vector3D t = a*(this->random01() - .5)*99 + b*(this->random01() - .5)*99;
          // add up the color value for this pixel
          p = p + this->sample(camPosition + t, !(t*-1 + (a*(this->random01() + x) + b*(y + this->random01()) + c)*16))*3.5;
        }
        
        std::cout << (char)(int)p.x << (char)(int)p.y << (char)(int)p.z; 
      }
    }
  }

private:
  enum TraceResult { sky, ground, sphere };

  void initScene(std::vector<std::string>& strScene)
  {
    for (int i = 0; i < 9; ++i)
    {
      intScene[8-i] = strtol(strScene[i].c_str(), NULL, 2);
    }
  }

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

    // iterate over columns of scene
    for (int column = 28; column >= 0; column--)
    {
      // iterate over rows of scene (there are 9)
      for (int row = 8; row >= 0; row--)
      {
        // check if there is a sphere in the row/column
        if (intScene[row] & (1 << column))
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

    TraceResult traceRes = this->trace(origin, direction, intersectDist, reflection);

    if (traceRes == TraceResult::sky)
    {
      // get a magic sky color 
      return Color(.7, .6, 1)*pow(1-direction.z, 4);
    }

    Vector3D intersectPoint = origin + direction*intersectDist;
    // what are those?
    Vector3D l = !(Vector3D(9 + this->random01(), 9 + this->random01(), 16) + intersectPoint*-1);
    Vector3D r = direction + reflection*((reflection%direction)*-2);

    // what is that?
    float b = l%reflection;

    if (b < 0 || TraceResult::sky != this->trace(intersectPoint, l, intersectDist, reflection))
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
    return Color(p, p, p) + this->sample(intersectPoint, r)*.5;
  }

  float random01() { return ((float)rand())/RAND_MAX; }


  int intScene[9];
  Vector3D camPosition;
  Vector3D camDirection;
};


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

  Raytracer tracer(scene);
  Vector3D pos = Vector3D(17, 32, 12);
  Vector3D dir = !Vector3D(-2, -16, 0);
  tracer.setCamPosition(pos);
  tracer.setCamDirection(dir);
  tracer.run();
}
