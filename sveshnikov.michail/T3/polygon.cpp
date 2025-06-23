#include "polygon.hpp"
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>

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

double sveshnikov::getPolygonArea(const Polygon &poly)
{
  const std::vector< Point > &pts = poly.points;
  std::vector< Point > next = pts;
  std::rotate(next.begin(), std::prev(next.end()), next.end());

  auto make_shoelace = std::bind(std::multiplies(), get_x, get_y);

  std::vector< double > left_shoelace_set;
  left_shoelace_set.reserve(pts.size());
  auto l_back_ins = std::back_inserter(left_shoelace_set);
  std::transform(pts.begin(), pts.end(), next.begin(), l_back_ins, make_shoelace);

  std::vector< double > right_shoelace_set;
  right_shoelace_set.reserve(pts.size());
  auto r_back_ins = std::back_inserter(right_shoelace_set);
  std::transform(next.begin(), next.end(), pts.begin(), r_back_ins, make_shoelace);

  auto l_begin_it = left_shoelace_set.begin();
  auto l_end_it = left_shoelace_set.end();
  auto r_begin_it = right_shoelace_set.begin();
  std::transform(l_begin_it, l_end_it, r_begin_it, l_begin_it, std::minus());
  double area = std::accumulate(l_begin_it, l_end_it, 0.0);
  return std::abs(area) / 2;
}

bool sveshnikov::operator==(const Point &pos1, const Point &pos2)
{
  return pos1.x == pos2.x && pos1.y == pos2.y;
}

bool sveshnikov::operator==(const Polygon &poly1, const Polygon &poly2)
{
  bool equal_size = poly1.points.size() == poly2.points.size();
  return equal_size && std::equal(poly1.points.begin(), poly1.points.end(), poly2.points.begin());
}
