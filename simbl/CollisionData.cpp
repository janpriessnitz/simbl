//
// Created by jp on 26.08.20.
//

#include "CollisionData.h"
#include "Util.h"
#include <algorithm>
#include <cmath>
#include <random>


CollisionData::CollisionData(Configuration *c) : config(c) {
  calculate_freqs();
  calculate_freqs_cum();
}

// TODO: precompute linear interpolation of frequency between two data points. Create a ~10x finer, bigger frequency array and do not use linear interpolation when simulating electron collisions. Instead, use the closest
// frequency data point. With fine enough frequency data point array, this should be almost as precise as the linear interpolation, but selecting the right collision will take O(log(c)) instead of O(log(nData)*c) (c is number of collisions).
// To use the binary search to determine the correct collision, we will need an array of "prefix sums" of frequencies of collisions.

void CollisionData::calculate_freqs() {
  for (unsigned int i = 0; i < Collision::nData; ++i) {
    freqs[i].resize(config->collisions.size());
    for (unsigned int j = 0; j < config->collisions.size(); ++j) {
      Molecule *mol = config->collisions[j]->molecule;
      freqs[i][j] = config->Ngas*mol->pp*config->collisions[j]->css[i]*util::electron_E_to_v(config->energy[i]);
    }
  }
}

// TODO: invert freqs_cum axes order for faster memory access
void CollisionData::calculate_freqs_cum() {
  for (unsigned int i = 0; i < Collision::nData; ++i) {
    freqs_cum[i].resize(config->collisions.size());
    freqs_cum[i][0] = freqs[i][0];
    for (unsigned int j = 1; j < config->collisions.size(); ++j) {
      freqs_cum[i][j] = freqs[i][j] + freqs_cum[i][j-1];
    }
  }
}

double CollisionData::max_tot_freq() {
  if(max_tot_freq_ != 0) return max_tot_freq_;
  double max_tot_freq = 0;
  for (unsigned int i = 0; i < Collision::nData; ++i) {
    if (freqs_cum[i][config->collisions.size() - 1] > max_tot_freq) max_tot_freq = freqs_cum[i][config->collisions.size() - 1];
  }
  max_tot_freq_ = max_tot_freq;
  return max_tot_freq_;
}

double CollisionData::get_okh(const double &en, const unsigned int &collision_id) {
  unsigned int lower_i, upper_i;
  double lower_p, upper_p;
  std::tie(lower_i, lower_p, upper_i, upper_p) = get_linear_interp_index_ratio(en);  // TODO: calculating the same thing twice in a row (first in get_freqs_cumulative)
  return config->collisions[collision_id]->okh[lower_i]*lower_p + config->collisions[collision_id]->okh[upper_i]*upper_p;
}

// Use the fact that all collisions' data points x-values are arranged to form an exponential ->
// we do not need to do binary search to find data point for a certain energy
// TODO: rewrite without any use of energy array
std::tuple<unsigned int, double, unsigned int, double> CollisionData::get_linear_interp_index_ratio(const double &en) {
  if (en <= config->energy[0]) return {0, 1, 0, 0};
  if (en >= config->energy[Collision::nData-1]) return {0, 0, Collision::nData-1, 1};

  double index = log2(en/(Collision::firstDataPointEnergy*util::q0))/log2(Collision::dataPointMult);
  unsigned int lower_i = floor(index);  // get 2 neighboring indexes
  unsigned int upper_i = ceil(index);
  double lower_portion = (en - config->energy[lower_i]) / (config->energy[upper_i] - config->energy[lower_i]);   // linear interpolation

  return {lower_i, lower_portion, upper_i, 1.0 - lower_portion};
}

int CollisionData::get_random_collision_experimental(const double &en) {
  std::uniform_real_distribution<> dist(0, max_tot_freq());
  double R = dist(util::rnd_eng);
  unsigned int lower_i, upper_i;
  double lower_p, upper_p;
  std::tie(lower_i, lower_p, upper_i, upper_p) = get_linear_interp_index_ratio(en);

  int n = config->collisions.size();
  int l = 0, r = n - 1;
  while (l < r) {
    int m  = (l+r)/2;
    if (freqs_cum[lower_i][m]*lower_p + freqs_cum[upper_i][m]*upper_p < R) {
      l = m + 1;
    } else {
      r = m;
    }
  }

  if (l >= n-1) return -1;
  if (config->collisions[l]->energy_threshold > en) {
    // try again
    return get_random_collision_experimental(en);
  }

//  printf("en %le %le %d\n", en, colls[l].energy_threshold, l);

  return l;

}

int CollisionData::get_random_collision(const double &en) {
  std::uniform_real_distribution<> dist(0, max_tot_freq());
  double R = dist(util::rnd_eng);
  std::vector<double> freqs_cumulative(config->collisions.size(), 0);
  unsigned int lower_i, upper_i;
  double lower_p, upper_p;
  std::tie(lower_i, lower_p, upper_i, upper_p) = get_linear_interp_index_ratio(en);

  for (unsigned int i = 0; i < config->collisions.size(); ++i) {
    if (config->collisions[i]->energy_threshold <= en)
      freqs_cumulative[i] = freqs[lower_i][i]*lower_p + freqs[upper_i][i]*upper_p;  // linear interpolation
    else
      freqs_cumulative[i] = 0;
    if (i != 0) {
      freqs_cumulative[i] += freqs_cumulative[i-1];
    }
  }
  auto it = std::upper_bound(freqs_cumulative.begin(), freqs_cumulative.end(), R);
  int res;
  if (it == freqs_cumulative.end()) res = -1;
  else res = it - freqs_cumulative.begin();

  return res;
}
