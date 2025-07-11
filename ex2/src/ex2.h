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
#ifndef EX2_H_
#define EX2_H_

#include "biodynamo.h"

namespace bdm {

inline int ex2(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound = -100.0;
    param->max_bound = +100.0;
    param->export_visualization = true;
    param->visualization_interval = 1;
    param->visualize_agents["Cell"] = { "diameter_", "density_", "volume_" };
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  Simulation sim(argc, argv, set_parameters);

  auto generate_grid_of_cells = [&](const Real3& xyz) {
    Cell* cell = new Cell();
    cell->SetDiameter(2.0);
    cell->SetDensity(1.0);
    cell->SetPosition(xyz);
    return cell;
  };
  size_t cells_per_axis = 5;
  real_t delta = 10.0;
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::Grid3D(cells_per_axis, delta, generate_grid_of_cells);

  sim.GetScheduler()->Simulate(10);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX2_H_
