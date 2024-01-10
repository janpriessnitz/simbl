//
// Created by jp on 21.09.20.
//

#include "ConfigLoader.h"
#include "Configuration.h"
#include "Simulation.h"

class TownsendSimulation : public Simulation {
public:
  TownsendSimulation(Configuration *c, double I0, unsigned int els_to_emit = 1000)
      : Simulation(c),
        I0(I0),
        els_to_emit(els_to_emit),
        n_els_emitted(0),
        n_els_landed(0)
  {
    fPosRecord = fopen("townsend_pos.out", "w");
    fVelRecord = fopen("townsend_vel.out", "w");
  }

  ~TownsendSimulation() {
    fclose(fPosRecord);
    fclose(fVelRecord);
  }

  virtual void boundary_actions() {
    for (unsigned int i = 0; i < electrons_->size(); ++i) {
      if (std::get<2>(electrons_->pos_[i]) > config_->width) {  // did I hit the anode?
        n_els_landed++;
        electrons_->remove(i);
      } else if (std::get<2>(electrons_->pos_[i]) < 0) {  // did I hit the cathode?
        // for now, let's pretend the cathode is transparent

        //        electrons_->remove(i);  // TODO: alternativelly n_els_emitted--;
      }
    }
  }

  virtual void timestep_actions() {
    if (els_to_emit <= n_els_emitted) return;  // do not emit any more electrons
    double n_emit_now = I0/util::q0*config_->dt;
    unsigned int whole_els = std::floor(n_emit_now);
    for (unsigned int i = 0; i < whole_els; ++i) {
      emit_electron();
      n_els_emitted++;
    }

    if (util::rnd_uni(0, 1) < n_emit_now - whole_els) {
      emit_electron();
      n_els_emitted++;
    }
  }

  void emit_electron() {
    double v_abs = sqrt(util::kb*config_->T_el_init/util::m0_el);
    double vx, vy, vz;
    vx = util::rnd_norm(0, 1)*v_abs;
    vy = util::rnd_norm(0, 1)*v_abs;
    vz = util::rnd_norm(0, 1)*v_abs;
    util::vec3d vel = {vx, vy, vz};

    // TODO: more sophisticated initial position
    double x, y, z;
    x = 0;
    y = 0;
    z = 0;

    util::vec3d pos = {x, y, z};
    electrons_->add(pos, vel);
  }

  void report_actions() override {
    Simulation::report_actions();
    fprintf(stderr, "landed: %u emitted: %u I/I0 ratio: %le\n", n_els_landed, n_els_emitted, n_els_landed/static_cast<double>(n_els_emitted));
    record_electron_pos(fPosRecord);
    record_electron_vel(fVelRecord);
  }

  bool isFinished() override {
    return els_to_emit <= n_els_emitted and electrons_->size() == 0;
  }

  void print_results() {
    // Ngas,E_plates_dist,width,els_emitted,els_landed
    printf("%le,%le,%le,%u,%u\n", config_->Ngas,
                                       -std::get<2>(config_->E_field),
                                       config_->width, n_els_emitted, n_els_landed);
  }

  double I0;
  unsigned int els_to_emit;

  unsigned int n_els_emitted;
  unsigned int n_els_landed;

  FILE *fPosRecord;
  FILE *fVelRecord;
};


int main(int argc, char **argv) {
  fprintf(stderr,"Running Townsend coefficient simulation\n");

  ConfigLoader loader;
  Configuration *config = loader.GetConfig(argv[1]);
//  Params params;

//  params.Ngas = strtod(argv[1], nullptr);
//  std::get<2>(params.E_field) = -strtod(argv[2], nullptr);
//
//  params.n_electrons_init = 0;
//  params.width = 1e-3;  // 1mm, enough to reach electron terminal velocity
//  params.reporting_period = 100000;

  TownsendSimulation sim(config, 1e-7, 1000); // 100 nA

  while(sim.Run());
  sim.print_results();
  return 0;
}
