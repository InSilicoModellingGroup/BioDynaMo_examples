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

/*
Create this enumerator to define (biochemical) substances that will
relate with the (agent) cell behvaior later in the agent-based model.
*/
enum Substances { kCytokine };

inline int ex8(int argc, const char* argv[]) {
  // https://biodynamo.github.io/api/structbdm_1_1Param.html
  /*
  Note below the insertion (by initialization) of some more global
  parameters, particularly the reaction-diffusion model data.
  */
  auto set_parameters = [](Param* param) {
    param->use_progress_bar = true;
    param->bound_space = Param::BoundSpaceMode::kClosed;
    param->min_bound =   0.0;
    param->max_bound = 100.0;
    param->export_visualization = true;
    param->visualization_interval = 10;
    param->visualize_agents["Cell"] = { "diameter_", "volume_" };
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
  /*
  Create a uniform (Cartesian) lattice of 51 times 51 times 51 vertices
  that will be used by a finite differences numerical model to calculate
  the reaction (see below the 'decay_rate' parameter) and diffusion (see
  below the 'diffusion_rate' parameter) of the corresponding substance.
  */
  ModelInitializer::DefineSubstance(kCytokine, "TGF", diffusion_rate, decay_rate, NxNxN);
  // https://biodynamo.github.io/api/namespacebdm.html
  const BoundaryConditionType bc_type = BoundaryConditionType::kNeumann;
  // https://biodynamo.github.io/api/structbdm_1_1ModelInitializer.html
  /*
  Indicate the appropriate boundary condition to apply at the uniform
  lattice in order to solve the reaction-diffusion equation for the
  above-mentioned substance.
  */
  ModelInitializer::AddBoundaryConditions(kCytokine, bc_type,
  // https://biodynamo.github.io/api/classbdm_1_1ConstantBoundaryCondition.html
                                          std::make_unique<ConstantBoundaryCondition>(0));

  /*
  Model parameters to control cells' behavior.
  */
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
    /*
    The customized cell migration behavior is identical to the previous
    example.
    */
    cell->AddBehavior(new MyMigration(migration_rate, propability, stick2boundary));
    // https://biodynamo.github.io/api/classbdm_1_1Secretion.html
    /*
    Incorporate the existing behavior of (biochemical) substance concentration
    modulation that indicates which substance to secrete (i.e., produce) or
    to update (i.e., reduce), and by a constant rate.
    */
    cell->AddBehavior(new Secretion("TGF", production_rate));
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

}  // namespace bdm

#endif  // EX8_H_
