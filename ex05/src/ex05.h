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
#ifndef EX05_H_
#define EX05_H_

#include "biodynamo.h"
/*
two user-defined header files are included here
*/
#include "my_growth_division.h"
#include "my_migration.h"

namespace bdm {

inline int ex05(int argc, const char* argv[]) {
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

  real_t max_diameter = 3.0;
  real_t volume_growth_rate = 0.05;
  real_t propability = 0.5;
  real_t migration_rate = 1.0;

  // https://biodynamo.github.io/api/classbdm_1_1Cell.html
  const real_t mean_xyz((param->max_bound+param->min_bound)/2);
  Cell* cell = new Cell({mean_xyz, mean_xyz, mean_xyz});
  cell->SetDiameter(2.0);
  cell->SetDensity(1.0);
  /*
  a user-defined behavior about the growth and division of a cell that
  follows that of example "ex4";
  however, check the 'my_growth_division.h' header file for more info
  */
  cell->AddBehavior(new MyGrowthDivision(max_diameter, volume_growth_rate, propability));
  /*
  a user-defined behavior that concerns the random movement of
  cells in 3D space by probing first (in every successive time-step
  of course) if the probability to move is below a threshold;
  however, do check the 'my_migration.h' header file for more info
  */
  cell->AddBehavior(new MyMigration(migration_rate, propability));
  rm->AddAgent(cell);

  sim.GetScheduler()->Simulate(1001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // EX05_H_
