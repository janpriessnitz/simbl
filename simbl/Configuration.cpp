//
// Created by jp on 09.12.20.
//

#include "Configuration.h"

Configuration::Configuration() = default;

Molecule *Configuration::findMolecule(const std::string &name) {
  for (const auto &mol : molecules) {
    if (mol->name == name) return mol;
  }
  return nullptr;
}
