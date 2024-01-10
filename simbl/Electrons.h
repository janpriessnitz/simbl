//
// Created by jp on 25.08.20.
//

#ifndef SIMBL_ELECTRONS_H
#define SIMBL_ELECTRONS_H

#include "Util.h"
#include <vector>

class Electrons {
public:
  std::vector<util::vec3d> pos_;
  std::vector<util::vec3d> vel_;
  std::vector<unsigned long long int> id_;
  unsigned long long int id_max_ = 0;

  void add(const util::vec3d &pos, const util::vec3d &vel);

  unsigned int size() const;

  std::string print(unsigned int index);

  // true is success, false is error
  bool remove(unsigned int index);
};


#endif//SIMBL_ELECTRONS_H
