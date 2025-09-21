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
  Simulation sim(argc, argv, set_parameters);
  // https://biodynamo.github.io/api/classbdm_1_1ResourceManager.html
  auto* rm = sim.GetResourceManager();
  // https://biodynamo.github.io/api/classbdm_1_1Random.html
  auto* rand = sim.GetRandom();

  const Real3 xyz{rand->Uniform(45.,55.),
                  rand->Uniform(45.,55.),
                  rand->Uniform(45.,55.)};

  // https://biodynamo.github.io/api/classbdm_1_1Cell.html
  Cell* cell = new Cell({0.0, 0.0, 0.0});
  cell->SetDiameter(2.0);
  cell->SetDensity(1.0);
  cell->SetPosition(xyz);
  rm->AddAgent(cell);

  sim.GetScheduler()->Simulate(10);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX1_H_
