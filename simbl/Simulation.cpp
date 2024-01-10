//
// Created by jp on 25.08.20.
//

#include "Simulation.h"
#include "CollisionData.h"
#include "Configuration.h"
#include <iostream>
#include <random>

Simulation::Simulation(Configuration *c) : config_(c), rnd_engine_(time(nullptr)) {
  electrons_ = new Electrons;
  excited_ = new ExcitedParticles;
  photons_ = new Photons;
  geometry_ = new BoxGeometry({config_->width, config_->height, config_->depth});
  coll_data_ = new CollisionData(config_);

  // TODO: in the original code, delta_MC_ is parametrized and dt is calculated (other way around)
  delta_MC_ = coll_data_->max_tot_freq()*config_->dt;

  spawn_init_electrons();

}

Simulation::~Simulation() {
  delete electrons_;
  delete excited_;
  delete photons_;
  delete geometry_;
  delete coll_data_;
}

bool Simulation::Run() {
  timestep_actions();

  move_photons();
  collide_photons();
  process_excited();

  accelerate_electrons();
  move_electrons();
  boundary_actions();
  collide_electrons();

  ++n_tsteps_;

  if (n_tsteps_ % config_->reporting_period == 0) {
    report_actions();
  }

  if (config_->remapping_threshold > 0 and electrons_->size() > config_->remapping_threshold) {
    remap_electrons();
  }

  return !isFinished();
}

bool Simulation::isFinished() {
  return n_tsteps_ >= config_->max_tsteps;
}

void Simulation::spawn_init_electrons() {
  for (unsigned int i = 0; i < config_->n_electrons_init; ++i) {
    double v_abs = sqrt(util::kb*config_->T_el_init/util::m0_el);

    double vx, vy, vz;
    vx = util::rnd_norm(0, 1)*v_abs;
    vy = util::rnd_norm(0, 1)*v_abs;
    vz = util::rnd_norm(0, 1)*v_abs;
    util::vec3d vel = {vx, vy, vz};
    util::vec3d pos = {0, 0, 0};
    electrons_->add(pos, vel);
  }
}

void Simulation::move_electrons() {
  for (unsigned int i = 0; i < electrons_->pos_.size(); ++i) {
    electrons_->pos_[i] = electrons_->pos_[i] + config_->dt*electrons_->vel_[i];
  }
}

void Simulation::accelerate_electrons() {
  for (unsigned int i = 0; i < electrons_->pos_.size(); ++i) {
    util::vec3d dv = - util::q0/(util::m0_el*util::get_lorentz(electrons_->vel_[i]))*config_->dt*config_->E_field;
    electrons_->vel_[i] = electrons_->vel_[i] + dv;
    // TODO: add pos += 1/2*acc*dt**2 ??
  }
}

void Simulation::remap_electrons() {
  for (unsigned int i = 0;i < electrons_->pos_.size(); ++i) {
    // TODO: complete this
  }
}

void Simulation::record_electron_pos(FILE *fp) {
  for (unsigned int i = 0; i < electrons_->size(); ++i) {
    double x, y, z;
    std::tie(x, y, z) = electrons_->pos_[i];
    fprintf(fp, "%u %llu %.17g %.17g %.17g\n", n_tsteps_, electrons_->id_[i], x, y, z);
  }
}

void Simulation::record_electron_vel(FILE *fp) {
  for (unsigned int i = 0; i < electrons_->size(); ++i) {
    double vx, vy, vz;
    std::tie(vx, vy, vz) = electrons_->vel_[i];
    fprintf(fp, "%u %llu %le %le %le\n", n_tsteps_, electrons_->id_[i], vx, vy, vz);
  }
}

void Simulation::collide_electrons() {
  auto collision_rand_dist = std::uniform_real_distribution<>(0, 1);

  for (int i = 0; i < electrons_->size(); ++i) {
    double collision_rand = collision_rand_dist(rnd_engine_);
    if (delta_MC_ < collision_rand) continue;  // to speed it up, we do this optimization: do not evaluate collisions too frequently,
    // since it would anyway end up mostly with null collisions

    double energy = util::electron_v_to_E(electrons_->vel_[i]);
    int collision_id = coll_data_->get_random_collision_experimental(energy);
    if (collision_id == -1) continue; // null collision
    ++n_collisions_;
    // TODO: what about negative energy thresholds in the collision data?
    double energy_threshold = config_->collisions[collision_id]->energy_threshold;
    double energy_gain = - energy_threshold;
    if (config_->collisions[collision_id]->type == kIonization) {
      double eps_bar = config_->collisions[collision_id]->molecule->eps_bar;
      double rnd = util::rnd_uni(0, 1);
      double energy_sec = eps_bar*tan(rnd*atan((energy - energy_threshold)/(2*eps_bar)));

      util::vec3d vel_sec = {util::rnd_norm(0, 1), util::rnd_norm(0, 1), util::rnd_norm(0, 1)};
      vel_sec = 1.0/util::abs(vel_sec)*vel_sec;  // normalize it
      vel_sec = util::electron_E_to_v(energy_sec)*vel_sec;  // match secondary electrons' speed to its energy
      electrons_->add(electrons_->pos_[i], vel_sec);
      energy_gain -= energy_sec;
    } else if (config_->collisions[collision_id]->type == kAttachment) {
      electrons_->remove(i);
      continue;
    } else if (config_->collisions[collision_id]->type == kExcitation) {
      if (config_->collisions[collision_id]->exc != nullptr) {
        excited_->add(electrons_->pos_[i], config_->collisions[collision_id]->exc);
      }
    }

    double rnd_theta_arg = util::rnd_uni(0, 1);

    double okh = coll_data_->get_okh(energy, collision_id);

    double theta = acos(1.0-2.0*rnd_theta_arg*(1.0-okh)/(1.0+okh*(1.0-2.0*rnd_theta_arg)));

    double phi = util::rnd_uni(0, 2*M_PI);

    util::vec3d gas_vel = {0, 0, 0};
    util::vec3d vel1_out, vel2_out;
    util::collision(electrons_->vel_[i],
                    gas_vel,
                    util::m0_el*util::get_lorentz(electrons_->vel_[i]),
                    config_->collisions[collision_id]->molecule->mass,
                    theta,
                    phi,
                    energy_gain,
                    vel1_out,
                    vel2_out);

    electrons_->vel_[i] = vel1_out;
  }
}

void Simulation::collide_photons() {
}

void Simulation::move_photons() {
}

void Simulation::process_excited() {
  auto rand_dist = std::uniform_real_distribution<>(0, 1);

  for (int i = 0; i < excited_->size(); ++i) {
    // Note: we are trying predissociation first and radiation after.
    // This is not mathematically correct, but does not have considerable effect with such a small timestep.
    double predis_rand = rand_dist(rnd_engine_);
    if (predis_rand < excited_->ex_[i]->predis_rate*config_->dt) {
      // predissociation
      excited_->remove(i);
      ++n_predis_;
      continue;
    }

    for (const auto &trans : excited_->ex_[i]->trans) {
      double rad_rand = rand_dist(rnd_engine_);
      if (rad_rand < trans->rad_rate*config_->dt) {
        // radiation
        photons_->spawn_anisotropic(excited_->pos_[i], trans);
        excited_->remove(i);
        break;
      }
    }
  }
}

double Simulation::mean_energy() {
  double energy = 0;
  for (int i = 0; i < electrons_->size(); ++i) {
    energy += util::electron_v_to_E(electrons_->vel_[i]);
  }
  energy /= electrons_->size()*util::q0;
  return energy;
}

double Simulation::drift_velocity() {
  double vel = 0;
  double energy = 0;
  for (int i = 0; i < electrons_->size(); ++i) {
    vel += std::get<2>(electrons_->vel_[i]);
  }
  vel /= electrons_->size();
  return vel;
}


void Simulation::boundary_actions() {
  for (unsigned int i = 0; i < electrons_->size(); ++i) {
    // still inside -> do nothing
    if (geometry_->isInside(electrons_->pos_[i])) continue;
    electrons_->remove(i);
  }
}

void Simulation::timestep_actions() {
}

void Simulation::report_actions() {
  fprintf(stderr,"Timestep: %u %le n_colls: %lld n_els: %u avg_energy: %le avg_vel: %le, n_excited: %lu, n_predis: %lu, n_photons: %lu\n",
          n_tsteps_, n_tsteps_*config_->dt, n_collisions_, electrons_->size(), mean_energy(), drift_velocity(), excited_->size(), n_predis_, photons_->size());
}

