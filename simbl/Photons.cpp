//
// Created by jp on 01.03.21.
//

#include "Photons.h"

void Photons::add(const util::vec3d &pos, const util::vec3d &vel, Transition *trans) {
  pos_.push_back(pos);
  vel_.push_back(vel);
  trans_.push_back(trans);
}

void Photons::remove(unsigned int index) {
  if (index >= pos_.size()) return;
  if (index != pos_.size() - 1) {
    pos_[index] = pos_[pos_.size() - 1];
    vel_[index] = vel_[vel_.size() - 1];
    trans_[index] = trans_[trans_.size() - 1];
  }
  pos_.pop_back();
  vel_.pop_back();
  trans_.pop_back();
}

unsigned long int Photons::size() {
  return pos_.size();
}
void Photons::spawn_anisotropic(const util::vec3d &pos, Transition *trans) {
  util::vec3d vel = random_photon_vel();
  printf("%lf: %lf %lf %lf\n", util::abs(vel), std::get<0>(vel), std::get<1>(vel), std::get<2>(vel));
  add(pos, vel, trans);
}

util::vec3d Photons::random_photon_vel() {

  util::vec3d vel = {
          util::rnd_norm(0, 1),
          util::rnd_norm(0, 1),
          util::rnd_norm(0, 1)
  };
  vel = (util::c_light/util::abs(vel))*vel;
  return vel;
}
