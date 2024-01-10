//
// Created by jp on 25.08.20.
//

#include "Cli.h"
#include "ConfigLoader.h"
#include "Configuration.h"
#include "Simulation.h"


#include <cstdio>

Cli::Cli(int argc, char **argv) {
  printf("Running SimBl version %s\n", util::simbl_version);
  ConfigLoader configLoader;
  Configuration *config = configLoader.GetConfig(argv[1]);

  Simulation sim(config);

  fprintf(stderr, "Starting simulation\n");
  while(sim.Run());
}


int main(int argc, char **argv) {
  Cli cli(argc, argv);
  return 0;
}
