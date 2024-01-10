//
// Created by jp on 14.12.20.
//

#include "ConfigLoader.h"
ConfigLoader::ConfigLoader() {
}
Configuration *ConfigLoader::GetConfig(const std::string &config_filename) {
  Configuration *conf = new Configuration();
  conf->config_path = config_filename;

  size_t found;
  found = config_filename.find_last_of("/\\");
  config_dir = config_filename.substr(0, found);

  MapReader reader(config_filename);

  conf->width = reader.GetFloat("width");
  conf->height = reader.GetFloat("height");
  conf->depth = reader.GetFloat("depth");
  conf->dt = reader.GetFloat("dt");
  double Ex = reader.GetFloat("E_field_x");
  double Ey = reader.GetFloat("E_field_y");
  double Ez = reader.GetFloat("E_field_z");
  conf->E_field = {Ex, Ey, Ez};
  conf->n_electrons_init = reader.GetInt("n_electrons_init");
  conf->T_el_init = reader.GetFloat("T_el_init");
  conf->max_tsteps = reader.GetInt("max_tsteps");
  conf->Ngas = reader.GetFloat("Ngas");
  conf->reporting_period = reader.GetInt("reporting_period");

  molecules_filename = GetActualPath(config_dir, reader.GetString("molecules_filename"));
  conf->molecules = GetMolecules();
  GetExcitations(conf);
  conf->collisions = GetCollisions(conf);

  return conf;
}

std::string ConfigLoader::GetActualPath(const std::string &prefix, const std::string &path) {
  if (path[0] == '/') return path;
  return prefix + "/" + path;
}
std::vector<Molecule *> ConfigLoader::GetMolecules() {
  std::vector<Molecule *> result;

  TupleReader reader(molecules_filename);
  for (size_t i = 0; i < reader.data.size(); ++i) {
    Molecule *mol = new Molecule;
    mol->name = reader.data[i][0];
    mol->mass = reader.GetDouble(i, 1)*util::amu;
    mol->eps_bar = reader.GetDouble(i, 2)*util::q0; // eV -> J
    mol->pp = reader.GetDouble(i, 3);
    mol->excitations_filename = GetActualPath(config_dir, reader.data[i][4]);
    mol->collisions_filename = GetActualPath(config_dir, reader.data[i][5]);
    result.push_back(mol);
  }
  return result;
}

void ConfigLoader::GetExcitations(Configuration *config) {
  for (auto &molecule : config->molecules) {
    TupleReader reader(molecule->excitations_filename);
    for (size_t i = 0; i < reader.data.size();) {
      Excitation *excitation = new Excitation;
      excitation->molecule = molecule;
      excitation->name = GetCanonicString(reader.data[i]);
      ++i;
      excitation->predis_rate = reader.GetDouble(i, 0);
      ++i;
      size_t n_trans = reader.GetInt(i, 0);
      ++i;
      for (size_t j = 0; j < n_trans; ++j) {
        Transition *trans = new Transition;
        trans->wavelength = reader.GetDouble(i, 0);
        trans->rad_rate = reader.GetDouble(i, 1);
        trans->self_abs_css = reader.GetDouble(i, 2);
        ++i;
        for (size_t k = 0; k < reader.data[i].size(); k += 2) {
          Molecule *mol = config->findMolecule(reader.data[i][k]);
          if (mol == nullptr) {
            fprintf(stderr, "Config: error when loading transition dissociation css: molecule %s not found\n", reader.data[i][k].c_str());
          }
          double dis_css = reader.GetDouble(i, k+1);
          trans->dis_css[mol] = dis_css;
        }
        ++i;
        for (size_t k = 0; k < reader.data[i].size(); k += 2) {
          Molecule *mol = config->findMolecule(reader.data[i][k]);
          if (mol == nullptr) {
            fprintf(stderr, "Config: error when loading transition photoionization css: molecule %s not found\n", reader.data[i][k].c_str());
          }
          double photoion_css = reader.GetDouble(i, k+1);
          trans->photoion_css[mol] = photoion_css;
        }
        excitation->trans.push_back(trans);
        ++i;
      }

      molecule->excitation_map[excitation->name] = excitation;
    }
  }
}

std::vector<Collision *> ConfigLoader::GetCollisions(Configuration *config) {
  std::vector<Collision *> result;
  for (auto &molecule : config->molecules) {
    TupleReader reader(molecule->collisions_filename);
    long long n_colls = reader.GetInt(0, 0);
    size_t row_begin = 1;

    for (unsigned int i = 0; i < n_colls; ++i) {
      auto *cur = new Collision;
      cur->desc = GetCanonicString(reader.data[row_begin]);
      switch (reader.GetInt(row_begin + 1, 0)) {
        case 1:
          cur->type = kElastic;
          break;
        case 2:
          cur->type = kExcitation;
          break;
        case 3:
          cur->type = kIonization;
          break;
        case 4:
          cur->type = kAttachment;
          break;
        default:
          cur->type = kElastic;
      }
      auto ex_name = GetCanonicString(reader.data[row_begin + 2]);
      if (!ex_name.empty() and molecule->excitation_map.count(ex_name) > 0) {
        cur->exc = molecule->excitation_map[ex_name];
      } else {
        cur->exc = nullptr;
      }
      cur->energy_threshold = reader.GetDouble(row_begin + 3, 0)*util::q0;
      cur->molecule = molecule;

      for (unsigned int j = 0; j < Collision::nData; ++j) {
        if (i == 0) {
          config->energy[j] = reader.GetDouble(row_begin + 4 + j, 0) * util::q0;// eV -> J
        }
        cur->css[j] = reader.GetDouble(row_begin + 4 + j, 1) * 1e-4;// cm -> m2
        cur->okh[j] = reader.GetDouble(row_begin + 4 + j, 2);
      }

      result.push_back(cur);
      row_begin += Collision::nData + 4;
    }
  }
  return result;
}
std::string ConfigLoader::GetCanonicString(const std::vector<std::string> &in){
  std::string res;
  for (auto &word : in) {
    res += word;
    res += " ";
  }
  if (!res.empty()) {
    res.pop_back();
  }
  return res;
}

