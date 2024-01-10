//
// Created by jp on 19.10.20.
//

#include "ConfigLoader.h"
#include "Configuration.h"
#include "Simulation.h"

#include <cmath>

class MossValidationSimulation : public Simulation {
public:
  explicit MossValidationSimulation(Configuration *c) : Simulation(c)
  {
    fPos = fopen("moss_pos.out", "w");
    fVel = fopen("moss_vel.out", "w");
    fStats = fopen("moss_stats.out", "w");

  }

  ~MossValidationSimulation() {
    fclose(fPos);
    fclose(fVel);
    fclose(fStats);
  }

  void report_actions() override {
    Simulation::report_actions();
    record_electron_vel(fVel);
    record_electron_pos(fPos);

    fprintf(fStats, "%le %le %le\n", n_tsteps_*config_->dt, mean_energy(), drift_velocity());
  }

//  bool isFinished() override {
//    // every 1000 steps, check if energy is constant for a while and end the simulation
//    if (n_tsteps_ % 1000 == 0) {
//      double en = mean_energy();
//
//      if (abs(en - last_mean_energy) < relative_error_threshold * en) {
//        current_constant_steps += 1;
//      } else {
//        current_constant_steps = 0;
//      }
//
//
//      last_mean_energy = en;
//
//      return current_constant_steps >= target_constant_steps;
//    } else {
//      return false;
//    }
//  }

  void boundary_actions() override {
  }

  const float relative_error_threshold = 0.005;
  const int target_constant_steps = 10000;
  int current_constant_steps = 0;
  double last_mean_energy{};

  FILE *fPos;
  FILE *fVel;
  FILE *fStats;
};


int main(int argc, char **argv) {
  ConfigLoader loader;
  Configuration *config = loader.GetConfig(argv[1]);
  MossValidationSimulation sim(config);

  while(sim.Run());

  return 0;
}
