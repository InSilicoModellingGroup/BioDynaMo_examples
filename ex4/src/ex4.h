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
#ifndef EX4_H_
#define EX4_H_

#include "biodynamo.h"
#include "my_growth_division.h"

namespace bdm {

inline int ex4(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound = -50.0;
    param->max_bound = +50.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["Cell"] = { "diameter_", "volume_" };
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  Simulation sim(argc, argv, set_parameters);

  auto* rm = sim.GetResourceManager();

  real_t max_diameter = 3.0;
  real_t volume_growth_rate = 0.12;
  real_t propability = 0.9;
  // https://biodynamo.github.io/api/classbdm_1_1Cell.html
  Cell* cell = new Cell({0.0, 0.0, 0.0});
  cell->SetDiameter(2.0);
  cell->SetDensity(1.0);
  // check the 'my_growth_division.h' header file
  cell->AddBehavior(new MyGrowthDivision(max_diameter, volume_growth_rate, propability));
  rm->AddAgent(cell);

  sim.GetScheduler()->Simulate(1001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX4_H_
