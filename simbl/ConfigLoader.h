//
// Created by jp on 14.12.20.
//

#ifndef SIMBL_CONFIGLOADER_H
#define SIMBL_CONFIGLOADER_H


#include "Configuration.h"
#include <string>

class ConfigLoader {
public:
  ConfigLoader();
  Configuration *GetConfig(const std::string &config_filename);

private:
  static std::string GetActualPath(const std::string &prefix, const std::string &path);
  static std::string GetCanonicString(const std::vector<std::string> &in);

  std::vector<Molecule *> GetMolecules();
  void GetExcitations(Configuration *config);
  std::vector<Collision *> GetCollisions(Configuration *config);


  std::string config_dir;
  std::string molecules_filename;
  std::string excitations_filename;
  std::string collisions_filename;
};


#endif//SIMBL_CONFIGLOADER_H
