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
#ifndef EX8_H_
#define EX8_H_

#include "biodynamo.h"
#include "my_growth.h"
#include "my_migration.h"

namespace bdm {

enum Substances { kCytokine };

inline int ex8(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound = -50.0;
    param->max_bound = +50.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["Cell"] = { "diameter_", "volume_" };
    param->visualize_diffusion = { Param::VisualizeDiffusion{"TGF", true, false} };
    param->calculate_gradients = false;
    param->diffusion_method = "euler";
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  Simulation sim(argc, argv, set_parameters);

  const Param* param = sim.GetParam();

  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  real_t diffusion_rate = 0.0;
  real_t decay_rate = 0.05e-3;
  int NxNxN = 51;
  ModelInitializer::DefineSubstance(kCytokine, "TGF", diffusion_rate, decay_rate, NxNxN);
  // https://biodynamo.github.io/api/namespacebdm.html
  const BoundaryConditionType bc_type = BoundaryConditionType::kNeumann;
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::AddBoundaryConditions(kCytokine, bc_type,
  // https://biodynamo.github.io/api/classbdm_1_1ConstantBoundaryCondition.html
                                          std::make_unique<ConstantBoundaryCondition>(0));

  real_t migration_rate = 1.0;
  real_t propability = 0.5;
  real_t production_rate = 0.2e-3;
  bool stick2boundary = true;

  auto generate_cluster_of_cells = [&](const Real3& xyz) {
    Cell* cell = new Cell();
    cell->SetDiameter(2.0);
    cell->SetDensity(1.0);
    cell->SetPosition(xyz);
    // check the 'my_migration.h' header file
    cell->AddBehavior(new MyMigration(migration_rate, propability, stick2boundary));
    // https://biodynamo.github.io/api/classbdm_1_1Secretion.html
    cell->AddBehavior(new Secretion("TGF", production_rate));
    return cell;
  };
  const Real3 center{0.0,0.0,0.0};
  const real_t radius(0.5*(param->max_bound-param->min_bound));
  ModelInitializer::CreateAgentsInSphereRndm(center,radius,2222, generate_cluster_of_cells);

  sim.GetScheduler()->Simulate(5001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX8_H_
