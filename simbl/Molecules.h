//
// Created by jp on 14.12.20.
//

#ifndef SIMBL_MOLECULES_H
#define SIMBL_MOLECULES_H

#include "Collisions.h"
#include "Excitations.h"
#include <string>
#include <unordered_map>

class Molecule {
public:
  Molecule() = default;
  std::string name;
  double mass;
  double pp;
  double eps_bar;

  std::unordered_map<std::string, Excitation *> excitation_map;

  std::string excitations_filename;
  std::string collisions_filename;
};

class Molecules {
public:
  Molecules();

private:

};


#endif//SIMBL_MOLECULES_H
