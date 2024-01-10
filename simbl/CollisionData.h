//
// Created by jp on 26.08.20.
//

#ifndef SIMBL_COLLISIONDATA_H
#define SIMBL_COLLISIONDATA_H

#include "Util.h"
#include <string>
#include <vector>

#include "Collisions.h"
#include "Configuration.h"

class CollisionData {
public:
  explicit CollisionData(Configuration *config);
  double max_tot_freq();

  int get_random_collision(const double& en);  // returns -1 if null collision
  int get_random_collision_experimental(const double &en);
  double get_okh(const double& en, const unsigned int &collision_id);

  std::vector<double> freqs[Collision::nData];
  std::vector<double> freqs_cum[Collision::nData];
  double energy[Collision::nData];

private:
  std::tuple<unsigned int, double, unsigned int, double> get_linear_interp_index_ratio(const double &en);
  void calculate_freqs();
  void calculate_freqs_cum();

  double max_tot_freq_ = 0;

  Configuration *config;
};

#endif//SIMBL_COLLISIONDATA_H
