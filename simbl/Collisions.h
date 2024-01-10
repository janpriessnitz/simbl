//
// Created by jp on 14.12.20.
//

#ifndef SIMBL_COLLISIONS_H
#define SIMBL_COLLISIONS_H

#include <string>

class Molecule;
class Excitation;

enum CollisionType {
  kElastic,
  kExcitation,
  kIonization,
  kAttachment
};

class Collision {
public:
  static const unsigned int nData = 10000; // number of data points for each collision
  constexpr static const double dataPointMult = 1.0027672000990759;  // ratio between collision energy data points a[i]/a[i-1] which is constant to all data
  constexpr static const double firstDataPointEnergy = 1.0e-3;  // energy of the first data point in the data

  CollisionType type;
  Molecule *molecule;
  double energy_threshold;
  std::string desc;
  Excitation *exc;

  double css[nData];
  double okh[nData];  // anisotropy, some Okhrimovsky angle magic
  double freq[nData];
};

class Collisions {
};


#endif//SIMBL_COLLISIONS_H
