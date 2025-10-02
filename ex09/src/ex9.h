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
#ifndef EX9_H_
#define EX9_H_

#include "biodynamo.h"
/*
Include a new header describing a new class of an agent (cell).
*/
#include "my_cell.h"
#include "my_growth.h"
#include "my_migration.h"

namespace bdm {

enum Substances { kCytokine };

inline int ex9(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  /*
  Please note the important differences on the visualization parameter
  'param->visualize_agents["MyCell"]' compared to the previous example.
  */
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound =   0.0;
    param->max_bound = 100.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["MyCell"] = { "diameter_", "volume_", "phenotype_" };
    param->visualize_diffusion = { Param::VisualizeDiffusion{"TGF", true, false} };
    param->calculate_gradients = false;
    param->diffusion_method = "euler";
    param->statistics = false;
    param->simulation_time_step = 1.0;
  };

  // https://biodynamo.github.io/api/classbdm_1_1Simulation.html
  Simulation sim(argc, argv, set_parameters);
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  const Param* param = sim.GetParam();

  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  real_t diffusion_rate = 0.0;
  real_t decay_rate = 0.05e-3;
  int NxNxN = 51;
  ModelInitializer::DefineSubstance(kCytokine, "TGF", diffusion_rate, decay_rate, NxNxN);
  const BoundaryConditionType bc_type = BoundaryConditionType::kNeumann;
  ModelInitializer::AddBoundaryConditions(kCytokine, bc_type,
                                          std::make_unique<ConstantBoundaryCondition>(0));

  /*
  Model parameters to control cells' behavior.
  */
  real_t uptake_rate = -0.2e-3;
  real_t migration_rate = 1.0;
  real_t propability = 0.5;
  real_t production_rate = 0.2e-3;
  bool stick2boundary = true;

  /*
  Used-defined function used below to generate cells. Note that these
  cells will be labelled (following the properties of the new cell type)
  as phenotype-1. This type of cells can only uptake "TGF".
  */
  auto generate_grid_of_cells = [&](const Real3& xyz) {
    MyCell* cell = new MyCell();
    cell->SetDiameter(4.0);
    cell->SetDensity(10.0);
    cell->SetPosition(xyz);
    cell->SetPhenotype(1);
    cell->AddBehavior(new Secretion("TGF", uptake_rate));
    return cell;
  };
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  const real_t min_location(0.9*param->min_bound);
  const real_t max_location(0.9*param->max_bound);
  /*
  Generate and add in the simulation engine 777 cells of phenotype-1.
  */
  ModelInitializer::CreateAgentsRandom(min_location,max_location,777, generate_grid_of_cells);

  /*
  Used-defined function used below to generate cells. Note that these
  cells will be labelled (following the properties of the new cell type)
  as phenotype-2. This type of cells can secrete "TGF" and migrate.
  */
  auto generate_cluster_of_cells = [&](const Real3& xyz) {
    MyCell* cell = new MyCell();
    cell->SetDiameter(2.0);
    cell->SetDensity(1.0);
    cell->SetPosition(xyz);
    cell->SetPhenotype(2);
    cell->AddBehavior(new MyMigration(migration_rate, propability, stick2boundary));
    cell->AddBehavior(new Secretion("TGF", production_rate));
    return cell;
  };
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  const real_t mean_xyz((param->max_bound+param->min_bound)/2);
  const Real3 center{mean_xyz, mean_xyz, mean_xyz};
  const real_t radius(0.5*(param->max_bound-param->min_bound));
  /*
  Generate and add in the simulation engine 2222 cells of phenotype-2.
  */
  ModelInitializer::CreateAgentsInSphereRndm(center,radius,2222, generate_cluster_of_cells);

  sim.GetScheduler()->Simulate(5001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

}  // namespace bdm

#endif  // EX9_H_
