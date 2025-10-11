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
#ifndef EX03_H_
#define EX03_H_

#include "biodynamo.h"

namespace bdm {

inline int ex03(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound =   0.0;
    param->max_bound = 100.0;
    param->export_visualization = true;
    param->visualization_interval = 1;
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
  Below we provide some model parameters that will influece the
  behaviour of a cell to grow (enlarge in size), and divide (split
  in two new cells).
  */
  real_t max_diameter = 3.0;
  real_t volume_growth_rate = 2.0;
  /*
  This is just a mean of the BioDynaMo simulation domain ;)
  */
  const real_t mean_xyz((param->max_bound+param->min_bound)/2);

  // https://biodynamo.github.io/api/classbdm_1_1Cell.html
  /*
  Simply create just one cell.
  */
  Cell* cell = new Cell({mean_xyz, mean_xyz, mean_xyz});
  cell->SetDiameter(2.0);
  cell->SetDensity(1.0);
  /*
  Not only the user can provide value-related data (like diameter,
  volume, etc.) but also they can plug in the cell "behavior" data.
  Below, the existing behavior model of growth and division is
  adopted. By inspection of the BioDynaMo source code, this behavior
  assumes a cell to grow (by a constant volume rate); when its
  diameter hits a critical value then it splits and creates a new cell.
  Notably only the original cell (mother cell) can continue to grow and
  divide, yet the new cell (daughter cell) simply lives on.
  */
  // https://biodynamo.github.io/api/classbdm_1_1GrowthDivision.html
  cell->AddBehavior(new GrowthDivision(max_diameter, volume_growth_rate));
  /*
  And now update the resource manager by adding the cell.
  */
  rm->AddAgent(cell);

  /*
  Notably the simulation progresses for 1001 steps this time.
  */
  sim.GetScheduler()->Simulate(1001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // EX03_H_
