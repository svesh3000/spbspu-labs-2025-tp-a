#include "commands.hpp"
#include <map>
#include <string>
#include <iomanip>
#include <numeric>
#include <functional>
#include <stream-guard.hpp>

namespace
{
  using is_t = std::istream;
  using os_t = std::ostream;
  using polygon_set_t = std::vector< sveshnikov::Polygon >;
  using param_holder_t = std::map< std::string, std::function< void() > >;

  bool isEven(const sveshnikov::Polygon &poly);
  size_t getNumVertexes(const sveshnikov::Polygon &poly);
  std::vector< double > getAreaSet(const polygon_set_t &shapes);
  std::vector< size_t > getVertexesSet(const polygon_set_t &shapes);

  double area_impl(const polygon_set_t &shapes);
  double area_even(const polygon_set_t &shapes);
  double area_odd(const polygon_set_t &shapes);
  double area_mean(const polygon_set_t &shapes);
  double area_num_vertexes(const polygon_set_t &shapes, size_t num);

  void max_area(os_t &out, const polygon_set_t &shapes);
  void max_vertexes(os_t &out, const polygon_set_t &shapes);

  void min_area(os_t &out, const polygon_set_t &shapes);
  void min_vertexes(os_t &out, const polygon_set_t &shapes);

  void count_even(os_t &out, const polygon_set_t &shapes);
  void count_odd(os_t &out, const polygon_set_t &shapes);
  void count_num_vertexes(os_t &out, const polygon_set_t &shapes, size_t num);

  bool isEven(const sveshnikov::Polygon &poly)
  {
    return poly.points.size() % 2 == 0;
  }

  size_t getNumVertexes(const sveshnikov::Polygon &poly)
  {
    return poly.points.size();
  }

  std::vector< double > getAreaSet(const polygon_set_t &shapes)
  {
    using namespace sveshnikov;
    std::vector< double > areas;
    areas.reserve(shapes.size());
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(areas), getPolygonArea);
    return areas;
  }

  std::vector< size_t > getVertexesSet(const polygon_set_t &shapes)
  {
    std::vector< size_t > areas;
    areas.reserve(shapes.size());
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(areas), getNumVertexes);
    return areas;
  }

  double area_impl(const polygon_set_t &shapes)
  {
    std::vector< double > areas = getAreaSet(shapes);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double area_even(const polygon_set_t &shapes)
  {
    polygon_set_t pts;
    pts.reserve(shapes.size());
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(pts), isEven);
    return area_impl(pts);
  }

  double area_odd(const polygon_set_t &shapes)
  {
    return area_impl(shapes) - area_even(shapes);
  }

  double area_mean(const polygon_set_t &shapes)
  {
    if (shapes.empty())
    {
      throw std::out_of_range("Error: no shapes!");
    }
    return area_impl(shapes) / shapes.size();
  }

  double area_num_vertexes(const polygon_set_t &shapes, size_t num)
  {
    using namespace std::placeholders;
    polygon_set_t pts;
    auto num_vert = std::bind(std::equal_to< size_t >(), std::bind(getNumVertexes, _1), num);
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(pts), num_vert);
    return area_impl(pts);
  }

  void max_area(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< double > areas = getAreaSet(shapes);
    auto it = std::max_element(areas.begin(), areas.end());
    sveshnikov::StreamGuard streamguard(out);
    out << std::fixed << std::setprecision(1) << *it << '\n';
  }

  void max_vertexes(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< size_t > areas = getVertexesSet(shapes);
    auto it = std::max_element(areas.begin(), areas.end());
    sveshnikov::StreamGuard streamguard(out);
    out << std::fixed << std::setprecision(1) << *it << '\n';
  }

  void min_area(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< double > areas = getAreaSet(shapes);
    auto it = std::min_element(areas.begin(), areas.end());
    sveshnikov::StreamGuard streamguard(out);
    out << std::fixed << std::setprecision(1) << *it << '\n';
  }

  void min_vertexes(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< size_t > areas = getVertexesSet(shapes);
    auto it = std::min_element(areas.begin(), areas.end());
    sveshnikov::StreamGuard streamguard(out);
    out << std::fixed << std::setprecision(1) << *it << '\n';
  }
}

void sveshnikov::area(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< double() > > params;
  params["EVEN"] = std::bind(area_even, std::cref(shapes));
  params["ODD"] = std::bind(area_odd, std::cref(shapes));
  params["MEAN"] = std::bind(area_mean, std::cref(shapes));

  std::string parm;
  in >> parm;
  double area = 0.0;
  try
  {
    size_t len_str = 0;
    int num_vertexes = std::stoi(parm, &len_str);
    if (len_str != parm.size())
    {
      throw std::invalid_argument("Error: the parameter is not a number!");
    }
    area = area_num_vertexes(shapes, num_vertexes);
  }
  catch (const std::invalid_argument &e)
  {
    area = params.at(parm)();
  }

  StreamGuard streamguard(out);
  out << std::fixed << std::setprecision(1) << area << '\n';
}

void sveshnikov::max(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< void() > > params;
  params["AREA"] = std::bind(max_area, std::ref(out), std::cref(shapes));
  params["VERTEXES"] = std::bind(max_vertexes, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  if (shapes.empty())
  {
    throw std::out_of_range("Error: no shapes!");
  }
  params.at(parm)();
}

void sveshnikov::min(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< void() > > params;
  params["AREA"] = std::bind(min_area, std::ref(out), std::cref(shapes));
  params["VERTEXES"] = std::bind(min_vertexes, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  if (shapes.empty())
  {
    throw std::out_of_range("Error: no shapes!");
  }
  params.at(parm)();
}

void sveshnikov::count(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< void() > > params;
  params["EVEN"] = std::bind(count_even, std::ref(out), std::cref(shapes));
  params["ODD"] = std::bind(count_odd, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  try
  {
    size_t len_str = 0;
    int num_vertexes = std::stoi(parm);
    if (len_str != parm.size())
    {
      throw std::invalid_argument("Error: the parameter is not a number!");
    }
    count_num_vertexes(out, shapes, num_vertexes);
  }
  catch (const std::invalid_argument &e)
  {
    params.at(parm)();
  }
}

void sveshnikov::maxseq(is_t &in, os_t &out, const polygon_set_t &shapes)
{}

void sveshnikov::rmecho(is_t &in, os_t &out, const polygon_set_t &shapes)
{}
