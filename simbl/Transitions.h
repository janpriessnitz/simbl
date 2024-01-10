//
// Created by jp on 01.03.21.
//

#ifndef SIMBL_TRANSITIONS_H
#define SIMBL_TRANSITIONS_H


#include <unordered_map>

class Excitation;
class Molecule;

class Transition {
public:
  Transition() = default;

  Excitation *exc = nullptr;
  double rad_rate;  // radiation rate s^-1
  double wavelength;  // nm
  // TODO: take into account lower vibrational state of the transition and Boltzmann distribution of background N2/O2 vibrational states
  double self_abs_css;  // m^2

  std::unordered_map<Molecule *, double> dis_css;  // dissociation cross section m^2
  std::unordered_map<Molecule *, double> photoion_css;  // photoionization cross section m^2

protected:
};


#endif//SIMBL_TRANSITIONS_H
