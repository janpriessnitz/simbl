//
// Created by jp on 25.08.20.
//

#ifndef SIMBL_EFIELD_H
#define SIMBL_EFIELD_H

#include "Util.h"

class EField {
public:
  EField() = default;
  virtual util::vec3d getAt(const util::vec3d &pos) = 0;
};

class ConstantEField : public EField {
public:
  explicit ConstantEField(const util::vec3d &intensity);
  util::vec3d getAt(const util::vec3d &pos) override;

private:
  util::vec3d intensity_;
};


#endif//SIMBL_EFIELD_H
