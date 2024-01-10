//
// Created by jp on 17.11.20.
//

#ifndef SIMBL_EXCITEDPARTICLES_H
#define SIMBL_EXCITEDPARTICLES_H

#include "Excitations.h"
#include "Util.h"
#include <vector>

class ExcitedParticles {
public:

  void add(const util::vec3d &pos, Excitation* ex);
  void remove(unsigned int index);

  unsigned long int size();

  std::vector<util::vec3d> pos_;
  std::vector<Excitation *> ex_;
};


#endif//SIMBL_EXCITEDPARTICLES_H
