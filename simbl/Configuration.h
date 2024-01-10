//
// Created by jp on 09.12.20.
//

#ifndef SIMBL_CONFIGURATION_H
#define SIMBL_CONFIGURATION_H

#include <string>

#include "MapReader.h"
#include "Molecules.h"
#include "Util.h"

class Configuration {
public:
  Configuration();

  Molecule *findMolecule(const std::string &name);

  double width = 1e5;  // 1mm, plates distance
  double height = 1e5;  // 3mm
  double depth = 0.003;  // 3mm
  double dt = 7.2316146052662784e-15;  // taken from zbona's work. To be determined
  util::vec3d E_field = {0, 0, -32e5*1};  // 0.3*Ek
  unsigned int n_electrons_init = 1000;
  double T_el_init = 0.5*util::q0/util::kb;
  unsigned int max_tsteps = 5000000;  // 5M

  double Ngas = 2.688e25;  // air density at ground pressure m^{-3}
  double ppN2 = 0.8;
  double ppO2 = 0.2;
  unsigned int remapping_threshold = -1;

  unsigned int reporting_period = max_tsteps/50;

  std::vector<Molecule *> molecules;

  std::vector<Collision *> collisions;
  double energy[Collision::nData];

  std::string config_path;
};


#endif//SIMBL_CONFIGURATION_H
