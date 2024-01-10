//
// Created by jp on 25.08.20.
//

#include "EField.h"

ConstantEField::ConstantEField(const util::vec3d &intensity) {
  intensity_ = intensity;
}

util::vec3d ConstantEField::getAt(const util::vec3d &pos) {
  return intensity_;
}

