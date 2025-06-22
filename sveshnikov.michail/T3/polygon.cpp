#include "polygon.hpp"

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

std::istream &sveshnikov::operator>>(std::istream &in, Point &&pos)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using sep = DelimiterIO;

  in >> std::noskipws;
  in >> sep{'('} >> pos.x >> sep{';'} >> pos.y >> sep{')'};
  in >> std::skipws;
}

std::istream &sveshnikov::operator>>(std::istream &in, Polygon &&shape)
{}

void sveshnikov::loadPolygons(std::istream &in, std::vector< Polygon > &shapes)
{}
