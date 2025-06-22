#include "polygon.hpp"
#include <iterator>
#include <algorithm>

namespace
{
  struct DelimiterIO
  {
    char exp;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

std::istream &sveshnikov::operator>>(std::istream &in, Point &pos)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using sep = DelimiterIO;
  Point p;

  in >> std::noskipws;
  in >> sep{'('} >> p.x >> sep{';'} >> p.y >> sep{')'};
  in >> std::skipws;

  if (in)
  {
    pos = std::move(p);
  }
}

std::istream &sveshnikov::operator>>(std::istream &in, Polygon &shape)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using in_iter = std::istream_iterator< Point >;
  Polygon poly;

  size_t num_points = 0;
  in >> num_points;
  std::copy_n(in_iter(in), num_points, std::back_inserter(poly));
  if (num_points < 3 || poly.points.size() != num_points || in.peek() != '\n')
  {
    in.setstate(std::ios::failbit);
  }

  if (in)
  {
    shape = std::move(poly);
  }
  return in;
}

void sveshnikov::loadPolygons(std::istream &in, std::vector< Polygon > &shapes)
{
  using in_iter = std::istream_iterator< Polygon >;
  using out_iter = std::ostream_iterator< Polygon >;

  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }
    std::copy(in_iter(std::cin), in_iter(), std::back_inserter(shapes));
  }
}
