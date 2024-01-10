//
// Created by jp on 01.03.21.
//

#ifndef SIMBL_PHOTONS_H
#define SIMBL_PHOTONS_H

#include "Transitions.h"
#include "Util.h"

#include <vector>

class Photons {
public:
  static util::vec3d random_photon_vel();

  std::vector<util::vec3d> pos_;
  std::vector<util::vec3d> vel_;
  std::vector<Transition *> trans_;

  void add(const util::vec3d &pos, const util::vec3d &vel, Transition *trans);
  void remove(unsigned int index);
  unsigned long int size();

  void spawn_anisotropic(const util::vec3d &pos, Transition *trans);
};


#endif//SIMBL_PHOTONS_H
