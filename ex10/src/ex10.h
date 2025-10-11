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

#include "biodynamo.h"
#include "core/behavior/secretion.h"
/*
Include a new header describing a new class of an agent (cell).
*/
#include "my_cell.h"

namespace bdm {

/*
Create this enumerator to define (biochemical) substances that will
relate with the (agent) cell behvaior later in the agent-based model.
*/
enum Substances { kCytokine };

inline int ex10(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound =   0.0;
    param->max_bound = 100.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["MyCell"] = { "diameter_", "volume_", "phenotype_" };
    param->visualize_diffusion = { Param::VisualizeDiffusion{"TGF", true, true} };
    param->calculate_gradients = false;
    param->diffusion_method = "euler";
    param->statistics = false;
    param->simulation_time_step = 0.5;
  };

  // https://biodynamo.github.io/api/classbdm_1_1Simulation.html
  Simulation sim(argc, argv, set_parameters);
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  const Param* param = sim.GetParam();

  const real_t DT = param->simulation_time_step;
  int NxNxN = 91;
  const real_t domain_center = 0.5*(param->max_bound+param->min_bound);
  const real_t domain_delta = 0.5*(param->max_bound-param->min_bound);

  /*
  Create a uniform (Cartesian) lattice of 51 times 51 times 51 vertices
  that will be used by a finite differences numerical model to solve
  the reaction-diffusion problem (below the first parameter corresponds
  to the diffusion rate while the second to the decay rate' parameter) of
  the corresponding substance.
  */
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::DefineSubstance(kCytokine, "TGF", 0.2/DT, 0.0/DT, NxNxN);
  /*
  Indicate the appropriate boundary condition to apply at the uniform
  lattice in order to solve the reaction-diffusion equation for the
  above-mentioned substance.
  */
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::AddBoundaryConditions(kCytokine,
  // https://biodynamo.github.io/api/namespacebdm.html
                                          BoundaryConditionType::kNeumann,
  // https://biodynamo.github.io/api/classbdm_1_1ConstantBoundaryCondition.html
                                          std::make_unique<ConstantBoundaryCondition>(0));

  /*
  User-defined function utlized below to generate cells. Note that these
  cells will be labelled (following the properties of the new cell type)
  as phenotype-1. This type of cells can only produce "TGF".
  */
  auto generate_grid_of_cells_1 = [&](const Real3& xyz) {
    // cell behavior model parameters
    real_t uptake_rate = -1.0/DT;

    MyCell* cell = new MyCell();
    cell->SetDiameter(1.0);
    cell->SetPosition(xyz);
    cell->SetPhenotype(1);
    cell->AddBehavior(new Secretion("TGF", uptake_rate));
    return cell;
  };
  /*
  Generate and add in the simulation engine 100 cells of phenotype-1.
  */
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::CreateAgentsRandom(domain_center-0.9*domain_delta,domain_center+0.9*domain_delta,
                                       5000, generate_grid_of_cells_1);

  /*
  User-defined function utlized below to generate cells. Note that these
  cells will be labelled (following the properties of the new cell type)
  as phenotype-2. This type of cells can only uptake "TGF".
  */
  auto generate_grid_of_cells_2 = [&](const Real3& xyz) {
    // cell behavior model parameters
    real_t produce_rate = +0.1/DT;

    MyCell* cell = new MyCell();
    cell->SetDiameter(2.0);
    cell->SetPosition(xyz);
    cell->SetPhenotype(2);
    cell->AddBehavior(new Secretion("TGF", produce_rate));
    return cell;
  };
  /*
  Generate and add in the simulation engine 1000 cells of phenotype-2.
  */
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  ModelInitializer::CreateAgentsRandom(domain_center-0.5*domain_delta,domain_center+0.5*domain_delta,
                                       100, generate_grid_of_cells_2);

  sim.GetScheduler()->Simulate(2001);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // EX10_H_
