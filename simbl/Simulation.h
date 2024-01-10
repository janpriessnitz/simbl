//
// Created by jp on 25.08.20.
//

#ifndef SIMBL_SIMULATION_H
#define SIMBL_SIMULATION_H

#include "CollisionData.h"
#include "Configuration.h"
#include "EField.h"
#include "Electrons.h"
#include "ExcitedParticles.h"
#include "Geometry.h"
#include "Photons.h"
#include "Util.h"
#include <random>

class Simulation {
public:
  /*
   * Takes Params object, which contains all necessary configuration
   */
  explicit Simulation(Configuration *c);
  ~Simulation();
  /*
   * Conducts one timestep of simulation. Should be called repeatedly
   * as long as the simulation is not finished.
   * Returns false if the simulation is finished, otherwise true.
   */
  virtual bool Run();

protected:
  void spawn_init_electrons();
  void accelerate_electrons();
  void move_electrons();
  void collide_electrons();
  void record_electron_pos(FILE *fp);

  void move_photons();
  void collide_photons();
  void process_excited();

  virtual void timestep_actions();
  virtual void boundary_actions();
  virtual void report_actions();


  unsigned int n_tsteps_ = 0;
  Electrons* electrons_;
  ExcitedParticles *excited_;
  Photons *photons_;
  Geometry* geometry_;
  // TODO: calculate E field
  EField* eField_{};
  Configuration *config_;

  CollisionData *coll_data_;
  double delta_MC_;

  long long int n_collisions_ = 0;
  unsigned long int n_predis_ = 0;

  std::default_random_engine rnd_engine_;
  void record_electron_vel(FILE *fp);
  virtual bool isFinished();
  double mean_energy();
  double drift_velocity();
  void remap_electrons();
};


#endif//SIMBL_SIMULATION_H
