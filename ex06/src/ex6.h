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
#ifndef EX6_H_
#define EX6_H_

#include "biodynamo.h"
#include "my_migration.h"

namespace bdm {

inline int ex6(int argc, const char* argv[]) {
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

  real_t migration_rate = 1.0;
  real_t propability = 0.5;
  bool stick2boundary = true;

  /*
  Used-defined function that is used below to generate cells provided
  some space vector, fixed properties and behavior.
  */
  auto generate_cluster_of_cells = [&](const Real3& xyz) {
    Cell* cell = new Cell();
    cell->SetDiameter(2.0);
    cell->SetDensity(1.0);
    cell->SetPosition(xyz);
    // check the 'my_migration.h' header file
    /*
    This version of the migration behavior function differs to that of example
    "ex5". This behavior includes the option to stick a cell and make it
    immobile in case it reaches the boundaries of the simulation domain.
    */
    cell->AddBehavior(new MyMigration(migration_rate, propability, stick2boundary));
    return cell;
  };
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  const real_t mean_xyz((param->max_bound+param->min_bound)/2);
  const Real3 center{mean_xyz, mean_xyz, mean_xyz};
  const real_t radius(0.45*(param->max_bound-param->min_bound));
  /*
  Execute an existing function that can create within a spherical region 2222
  randomly positioned agents (i.e., cells) while also using the user-defined
  function for customized cell creation. In this example, the center of the
  spherical region is the center of the BioDynaMo simulation domain.
  */
  ModelInitializer::CreateAgentsInSphereRndm(center,radius,2222, generate_cluster_of_cells);

  sim.GetScheduler()->Simulate(5001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX6_H_
