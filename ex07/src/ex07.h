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
#ifndef EX07_H_
#define EX07_H_

#include "biodynamo.h"
/*
two user-defined header files are included here
*/
#include "my_growth.h"
#include "my_migration.h"

namespace bdm {

inline int ex07(int argc, const char* argv[]) {
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
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  const Param* param = sim.GetParam();

  auto generate_cluster_of_cells = [&](const Real3& xyz) {
    // cell behavior model parameters
    real_t migration_rate = 1.0;
    real_t propability = 0.5;
    bool stick2boundary = true;

    Cell* cell = new Cell();
    cell->SetDiameter(2.0);
    cell->SetDensity(1.0);
    cell->SetPosition(xyz);
    /*
    Following from example "ex6", this migration behavior differs in
    such that if a cell reaches the domain boundary it sticks there, it
    stops moving anymore and then it starts growing until it reaches
    a maximum cell diameter value
    */
    cell->AddBehavior(new MyMigration(migration_rate, propability, stick2boundary));
    return cell;
  };
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  const real_t mean_xyz((param->max_bound+param->min_bound)/2);
  const Real3 center{mean_xyz, mean_xyz, mean_xyz};
  const real_t radius(0.45*(param->max_bound-param->min_bound));
  ModelInitializer::CreateAgentsInSphereRndm(center,radius,2222, generate_cluster_of_cells);

  sim.GetScheduler()->Simulate(5001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // EX07_H_
