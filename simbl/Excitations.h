//
// Created by jp on 14.12.20.
//

#ifndef SIMBL_EXCITATIONS_H
#define SIMBL_EXCITATIONS_H

#include "Transitions.h"

#include <string>
#include <vector>

class Molecule;

class Excitation {
public:
  Excitation() = default;

  std::string name;
  Molecule *molecule = nullptr;
  double predis_rate;  // s^-1
  std::vector<Transition *> trans;
};



#endif//SIMBL_EXCITATIONS_H
