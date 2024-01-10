//
// Created by jp on 25.08.20.
//

#include "Electrons.h"

#include <sstream>

void Electrons::add(const util::vec3d &pos, const util::vec3d &vel) {
  pos_.push_back(pos);
  vel_.push_back(vel);
  id_.push_back(++id_max_);
}

bool Electrons::remove(unsigned int index) {
  if (index >= pos_.size()) return false;
  if (index != pos_.size() - 1) {
    pos_[index] = pos_[pos_.size() - 1];
    vel_[index] = vel_[vel_.size() - 1];
    id_[index] = id_[id_.size() - 1];
  }
  pos_.pop_back();
  vel_.pop_back();
  id_.pop_back();
  return true;
}
unsigned int Electrons::size() const {
  return pos_.size();
}
std::string Electrons::print(unsigned int index) {
  std::stringstream ss;
  ss << "pos: " << std::get<0>(pos_[index]) << "," << std::get<1>(pos_[index]) << "," << std::get<2>(pos_[index]) <<
       " vel: " << std::get<0>(vel_[index]) << "," << std::get<1>(vel_[index]) << "," << std::get<2>(vel_[index]);

  return ss.str();
}
