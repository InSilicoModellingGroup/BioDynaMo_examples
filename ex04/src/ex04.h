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
#ifndef EX04_H_
#define EX04_H_

#include "biodynamo.h"
/*
a user-defined header file is included here
*/
#include "my_growth_division.h"

namespace bdm {

inline int ex04(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound =   0.0;
    param->max_bound = 100.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["Cell"] = { "diameter_", "volume_" };
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  // https://biodynamo.github.io/api/classbdm_1_1Simulation.html
  Simulation sim(argc, argv, set_parameters);
  // https://biodynamo.github.io/api/classbdm_1_1ResourceManager.html
  auto* rm = sim.GetResourceManager();
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  const Param* param = sim.GetParam();

  /*
  As with example "ex3", below we have some model parameters that will
  control the cell behavior.
  */
  real_t max_diameter = 3.0;
  real_t volume_growth_rate = 0.12;
  real_t propability = 0.9;
  const real_t mean_xyz((param->max_bound+param->min_bound)/2);

  // https://biodynamo.github.io/api/classbdm_1_1Cell.html
  Cell* cell = new Cell({mean_xyz, mean_xyz, mean_xyz});
  cell->SetDiameter(2.0);
  cell->SetDensity(1.0);
  // check the 'my_growth_division.h' header file
  /*
  Opposed to example "ex3", the user-defined growth & division behavior
  grows a cell until diameter reaches a max, and then it probes (in every
  successive time step) whether the probability to divide is below a
  threshold. If that's true then it splits into two cells both of which
  inherit this behavior; if not then nothing happens.
  */
  cell->AddBehavior(new MyGrowthDivision(max_diameter, volume_growth_rate, propability));
  rm->AddAgent(cell);

  sim.GetScheduler()->Simulate(1001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // EX04_H_
