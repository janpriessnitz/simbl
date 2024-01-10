//
// Created by jp on 25.08.20.
//

#include "Geometry.h"

BoxGeometry::BoxGeometry(double w, double h, double d) {
  w_ = w;
  h_ = h;
  d_ = d;
}
bool BoxGeometry::isInside(const util::vec3d &pos) {
  double x, y, z;
  std::tie(x, y, z) = pos;
  return (x > -w_/2 and x < w_/2 and y > -h_/2 and y < h_/2 and z > -d_/2 and z < d_/2);
}
