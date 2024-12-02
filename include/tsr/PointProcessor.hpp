#include "tsr/Point_3.hpp"

#include <CGAL/hierarchy_simplify_point_set.h>
#include <CGAL/tags.h>
#include <CGAL/wlop_simplify_and_regularize_point_set.h>

#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/jet_smooth_point_set.h>

namespace tsr {

Point_3 UTM_point_to_WGS84(Point_3 pointUTM, short zone,
                           bool isNorthernHemisphere);

Point_3 WGS84_point_to_UTM(Point_3 pointWGS84);

void jet_smooth_points(std::vector<Point_3> &points);
void simplify_points(std::vector<Point_3> &points);

} // namespace tsr