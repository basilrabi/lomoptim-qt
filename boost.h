#ifndef BOOST_H
#define BOOST_H

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>

typedef boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian> point_2d;
typedef std::pair<point_2d, unsigned long> point_value;
typedef boost::geometry::index::rtree<point_value, boost::geometry::index::quadratic<16>> rtree_t;

#endif // BOOST_H
