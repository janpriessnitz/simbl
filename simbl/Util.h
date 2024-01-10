//
// Created by jp on 25.08.20.
//

#ifndef SIMBL_UTIL_H
#define SIMBL_UTIL_H

#include <random>
#include <tuple>

namespace util {
  extern char simbl_version[];

  typedef std::tuple<double, double, double> vec3d;

  extern double c_light;
  extern double m0_el;
  extern double q0;
  extern double kb;
  extern double amu;
  extern double mN2;
  extern double mO2;
  extern double molecule_mass[];
  extern double energy_secondary_N2;
  extern double energy_secondary_O2;
  extern double molecule_eps_bar[];

  extern double abs(const vec3d& vel);
  extern double get_lorentz(const vec3d& vel);
  extern double get_lorentz(const double& vel_abs);
  extern double electron_E_to_v(const double& en);
  extern double electron_v_to_E(const vec3d& vel);
  extern double electron_v_to_E(const double& vel_abs);

  extern void collision(const vec3d &vel1_in, const vec3d &vel2_in,
                        const double &m1, const double &m2,
                        const double &theta, const double &phi, const double &energy_gain,
                        vec3d &vel1_out, vec3d &vel2_out);

  extern double rnd_uni(double min, double max);
  extern double rnd_norm(double mean, double stddev);

  extern std::default_random_engine rnd_eng;
}



extern util::vec3d operator+(const util::vec3d& a, const util::vec3d& b);

extern util::vec3d operator-(const util::vec3d& a, const util::vec3d& b);

extern util::vec3d operator*(const double& a, const util::vec3d& b);


#endif//SIMBL_UTIL_H
