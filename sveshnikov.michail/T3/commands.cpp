#include "commands.hpp"
#include <map>
#include <string>
#include <functional>

namespace
{
  using is_t = std::istream;
  using os_t = std::ostream;
  using polygon_set_t = std::vector< sveshnikov::Polygon >;
  using param_holder_t = std::map< std::string, std::function< void() > >;

  void area_even(os_t &out, const polygon_set_t &shapes);
  void area_odd(os_t &out, const polygon_set_t &shapes);
  void area_mean(os_t &out, const polygon_set_t &shapes);
  void area_num_vertexes(os_t &out, const polygon_set_t &shapes, size_t num);

  void max_area(os_t &out, const polygon_set_t &shapes);
  void max_vertexes(os_t &out, const polygon_set_t &shapes);

  void min_area(os_t &out, const polygon_set_t &shapes);
  void min_vertexes(os_t &out, const polygon_set_t &shapes);

  void count_even(os_t &out, const polygon_set_t &shapes);
  void count_odd(os_t &out, const polygon_set_t &shapes);
  void count_num_vertexes(os_t &out, const polygon_set_t &shapes, size_t num);
}

void sveshnikov::area(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  param_holder_t params;
  params["EVEN"] = std::bind(area_even, std::ref(out), std::cref(shapes));
  params["ODD"] = std::bind(area_odd, std::ref(out), std::cref(shapes));
  params["MEAN"] = std::bind(area_mean, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  try
  {
    size_t len_str = 0;
    int num_vertexes = std::stoi(parm, &len_str);
    if (len_str != parm.size())
    {
      throw std::invalid_argument("Error: the parameter is not a number!");
    }
    area_num_vertexes(out, shapes, num_vertexes);
  }
  catch (const std::invalid_argument &e)
  {
    params.at(parm)();
  }
}

void sveshnikov::max(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  param_holder_t params;
  params["AREA"] = std::bind(max_area, std::ref(out), std::cref(shapes));
  params["VERTEXES"] = std::bind(max_vertexes, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  params.at(parm)();
}

void sveshnikov::min(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  param_holder_t params;
  params["AREA"] = std::bind(min_area, std::ref(out), std::cref(shapes));
  params["VERTEXES"] = std::bind(min_vertexes, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  params.at(parm)();
}

void sveshnikov::count(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  param_holder_t params;
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
