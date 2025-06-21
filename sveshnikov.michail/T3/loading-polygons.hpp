#ifndef LOADING_POLYGONS_HPP
#define LOADING_POLYGONS_HPP
#include <iostream>
#include "polygon.hpp"

namespace sveshnikov
{
  void loadPolygons(std::istream &in, std::vector< Polygon > &shapes);
}

#endif
