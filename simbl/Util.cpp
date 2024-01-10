//
// Created by jp on 25.08.20.
//

#include "Util.h"

#include <cmath>
#include <iostream>

namespace util {

  char simbl_version[] = "0.0.1";

  double c_light = 299792458.0;  // speed of light
  double m0_el = 9.10938291e-31;  // electron's rest mass [kg]
  double q0 = 1.602176565e-19;  // elementary charge [Q]
  double kb = 1.3806488e-23;  // Boltzmann's constant
  double amu = 1.660538921e-27;  // atomic mass unit
  double mN2 = 2.0*14.007*amu;  // mass of N2 molecule [kg]
  double mO2 = 2.0*15.999*amu;  // mass of O2 molecule [kg]
  double molecule_mass[] = {mN2, mO2};
  // TODO: this should be in J, not eV, why??
  // taken from OPAL (Moss Table 4)
  double energy_secondary_N2 = 13.0*q0;  // eV
  double energy_secondary_O2 = 17.4*q0;  // eV
  double molecule_eps_bar[] = {energy_secondary_N2, energy_secondary_O2};


  double abs(const util::vec3d &vel) {
    double a, b, c;
    std::tie(a, b, c) = vel;
    return sqrt(a * a + b * b + c * c);
  }


  // TODO: try precomputing this for speedup, how precise should it be though?
  double get_lorentz(const util::vec3d &vel) {
    double v_abs = util::abs(vel);
    return get_lorentz(v_abs);
  }

  double get_lorentz(const double& vel_abs) {
    return 1.0 / sqrt(1 - vel_abs*vel_abs/c_light/c_light);
  }

  double electron_E_to_v(const double& en) {
    double E0_elec = m0_el*c_light*c_light;
    return c_light*sqrt(1.0-(E0_elec/(en + E0_elec))*(E0_elec/(en + E0_elec)));
  }

  double electron_v_to_E(const vec3d& vel) {
    return electron_v_to_E(util::abs(vel));
  }

  double electron_v_to_E(const double& vel_abs) {
    return m0_el*c_light*c_light*(get_lorentz(vel_abs)-1);
  }

  void collision(const vec3d &vel1_in, const vec3d &vel2_in,
                 const double &m1, const double &m2,
                 const double &theta, const double &phi, const double &energy_gain,
                 vec3d &vel1_out, vec3d &vel2_out) {
    double m12 = m1 + m2;
    double red_mass = m1*m2/m12;
    util::vec3d vt = (1/m12)*(m1*vel1_in+m2*vel2_in);
    util::vec3d vr = vel1_in - vel2_in;
    double vr_abs = util::abs(vr);

    double argument = vr_abs*vr_abs+2.0*energy_gain/red_mass;

    // TODO: ???
    if (argument < 0) argument = 0;

    double vr_abs_prime = sqrt(argument);

    vr = (1/vr_abs)*vr;  // normalize
    double A = sqrt(std::get<1>(vr)*std::get<1>(vr) + std::get<2>(vr)*std::get<2>(vr));
    double cr = 1.0;

    double cos_theta = cos(theta);
    double sin_theta = sin(theta);
    double sin_phi = sin(phi);
    double cos_phi = cos(phi);

    vec3d vr_out = {cos_theta*std::get<0>(vr) + sin_theta*sin_phi*A,
                    cos_theta*std::get<1>(vr) + sin_theta*(cr*std::get<2>(vr)*cos_phi - std::get<0>(vr)*std::get<1>(vr)*sin_phi)/A,
                    cos_theta*std::get<2>(vr) - sin_theta*(cr*std::get<1>(vr)*cos_phi + std::get<0>(vr)*std::get<2>(vr)*sin_phi)/A};

    vr_out = vr_abs_prime*vr_out;
    vel1_out = vt + m2/m12*vr_out;
    vel2_out = vt - m1/m12*vr_out;
  }

  double rnd_uni(double min, double max) {
    return std::uniform_real_distribution<>(min, max)(rnd_eng);
  }

  double rnd_norm(double mean, double stddev) {
    return std::normal_distribution<double>(mean, stddev)(rnd_eng);
  }

  std::default_random_engine rnd_eng(time(nullptr));

}

util::vec3d operator+(const util::vec3d& a, const util::vec3d& b) {
  return {std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b), std::get<2>(a) + std::get<2>(b)};
}

util::vec3d operator-(const util::vec3d& a, const util::vec3d& b) {
  return {std::get<0>(a) - std::get<0>(b), std::get<1>(a) - std::get<1>(b), std::get<2>(a) - std::get<2>(b)};
}

util::vec3d operator*(const double& a, const util::vec3d& b) {
  return {a*std::get<0>(b), a*std::get<1>(b), a*std::get<2>(b)};
}
