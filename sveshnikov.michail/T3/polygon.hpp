#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <iostream>
#include <vector>

namespace sveshnikov
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream &operator>>(std::istream &in, Point &&pos);
  std::istream &operator>>(std::istream &in, Polygon &&shape);

  void loadPolygons(std::istream &in, std::vector< Polygon > &shapes);
}

#endif
