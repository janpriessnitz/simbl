//
// Created by jp on 25.08.20.
//

#ifndef SIMBL_GEOMETRY_H
#define SIMBL_GEOMETRY_H

#include "Util.h"

class Geometry {
public:
  virtual bool isInside(const util::vec3d &pos) = 0;
};

class BoxGeometry : public Geometry {
public:
  BoxGeometry(double w, double h, double d);
  virtual bool isInside(const util::vec3d &pos);
  double w_;
  double h_;
  double d_;
};


#endif//SIMBL_GEOMETRY_H
