// -----------------------------------------------------------------------------
//
// Copyright (C) 2021 CERN & University of Surrey for the benefit of the
// BioDynaMo collaboration. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------
#ifndef EX1_H_
#define EX1_H_

#include "biodynamo.h"

namespace bdm {

inline int ex1(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  /*
  User-defined function that can be used to define the global
  parameters of a BioDynaMo simulation, such as the size of the
  simulation domain, a flag confirming that visualization data are
  exported and the frequency by which these data are saved in a
  Paraview file, the time increment, etc.
  */
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kOpen;
    param->min_bound =   0.0;
    param->max_bound = 100.0;
    param->export_visualization = true;
    param->visualization_interval = 1;
    param->visualize_agents["Cell"] = { "diameter_" };
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  // https://biodynamo.github.io/api/classbdm_1_1Simulation.html
  /*
  Vital creation of the BioDynaMo simulation engine object that
  handles everything about the platform. The construction below
  also takes care initializing the global parameters of the
  BioDynaMo simulation engine using the user-defined function
  above.
  */
  Simulation sim(argc, argv, set_parameters);
  // https://biodynamo.github.io/api/classbdm_1_1ResourceManager.html
  /*
  Access the data manager, or as it's termed in BioDynaMo, the resource
  manager of the simulation engine.
  */
  auto* rm = sim.GetResourceManager();
  // https://biodynamo.github.io/api/classbdm_1_1Random.html
  /*
  Access the random number generator facility.
  */
  auto* rand = sim.GetRandom();

  /*
  Simply create a 3-component array, i.e., a 3D vector of real-valued
  data that has been initialized by providing random numbers between
  45 to 55. This can be thought as a space vector with random numbers
  in the [45, 55] range.
  */
  const Real3 xyz{rand->Uniform(45.,55.),
                  rand->Uniform(45.,55.),
                  rand->Uniform(45.,55.)};

  // https://biodynamo.github.io/api/classbdm_1_1Cell.html
  /*
  Create an agent, here a BioDynaMo cell that is positioned in 3D
  space, initially at (0,0,0), the give some value to its diameter
  and "mass density" and update its position based using the above
  space vector with random numbers.
  */
  Cell* cell = new Cell({0.0, 0.0, 0.0});
  cell->SetDiameter(2.0);
  cell->SetDensity(1.0);
  cell->SetPosition(xyz);
  /*
  An important step to update the resource manager by adding the
  above agent (i.e., the cell) into the simulation engine.
  */
  rm->AddAgent(cell);

  /*
  Ask the BioDynaMo simulation engine to progress for 10 steps,
  where each step separated by the other by an an increment of 1
  (see parameter 'param->simulation_time_step' above).
  */
  sim.GetScheduler()->Simulate(10);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX1_H_
