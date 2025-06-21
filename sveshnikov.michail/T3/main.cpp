#include <fstream>
#include "loading-polygons.hpp"

int main(int argc, char **argv)
{
  using namespace sveshnikov;
  if (argc != 1)
  {
    std::cerr << "Error: file is not specified!\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "Error: cannot open file!\n";
    return 1;
  }
  std::vector< Polygon > shapes;
  loadPolygons(in, shapes);
}
