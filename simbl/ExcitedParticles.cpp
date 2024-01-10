//
// Created by jp on 17.11.20.
//

#include "ExcitedParticles.h"

void ExcitedParticles::add(const util::vec3d &pos, Excitation *ex) {
  pos_.push_back(pos);
  ex_.push_back(ex);
}
void ExcitedParticles::remove(unsigned int index) {
  if (index >= pos_.size()) return;
  if (index != pos_.size() - 1) {
    pos_[index] = pos_[pos_.size() - 1];
    ex_[index] = ex_[ex_.size() - 1];
  }
  pos_.pop_back();
  ex_.pop_back();
}
unsigned long int ExcitedParticles::size() {
  return pos_.size();
}
