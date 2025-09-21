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
#ifndef EX10_H_
#define EX10_H_

#include "my_utils.h"
#include "my_cell.h"
#include "my_migration.h"
#include "my_growth_division.h"

namespace bdm {

inline int ex10(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound =    0.0;
    param->max_bound = +100.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["MyCell"] = { "diameter_", "volume_", "phenotype_" };
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  Simulation sim(argc, argv, set_parameters);

  const Param* param = sim.GetParam();

  auto generate_array_of_cells = [&](const Real3& xyz) {
    real_t migration_rate = 1.0;
    real_t migration_propability = 0.0;

    MyCell* cell = new MyCell();
    cell->SetDiameter(4.0);
    cell->SetDensity(10.0);
    cell->SetPosition(xyz);
    cell->SetPhenotype(1);
    cell->AddBehavior(new MyMigration(migration_rate, migration_propability));
    return cell;
  };
  const size_t agents_per_dim = 7;
  const real_t space = (param->max_bound-param->min_bound)/(agents_per_dim-1);
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::Grid3D(agents_per_dim,space, generate_array_of_cells);

  auto generate_cluster_of_cells = [&](const Real3& xyz) {
    real_t volume_growth_rate = 0.025;
    real_t division_propability = 1.0;
    // scaled mean diameter between the two cell types
    real_t smallest_distance = 1.8 * ((2.0+4.0)/2.0);
    // maximum safe distance between cells; check what happens if you set it to zero
    const real_t safe_distance = 4.0;

    MyCell* cell = new MyCell();
    cell->SetDiameter(2.0);
    cell->SetDensity(1.0);
    cell->SetPosition(xyz);
    cell->SetPhenotype(2);
    cell->AddBehavior(new MyGrowthDivision(3.0, volume_growth_rate, division_propability, smallest_distance, safe_distance));
    return cell;
  };
  const real_t mean_xyz = (param->max_bound+param->min_bound)/2;
  const Real3 center{mean_xyz,mean_xyz,mean_xyz};
  const real_t radius(0.1*(param->max_bound-param->min_bound));
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::CreateAgentsInSphereRndm(center,radius,5, generate_cluster_of_cells);

  sim.GetScheduler()->Simulate(3001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX10_H_
