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
#ifndef MY_MIGRATION_H_
#define MY_MIGRATION_H_

#include "core/behavior/behavior.h"
#include "core/behavior/secretion.h"

namespace bdm {

class MyMigration : public Behavior {
  BDM_BEHAVIOR_HEADER(MyMigration, Behavior, 1);

  public:
    MyMigration() { AlwaysCopyToNew(); }
    MyMigration(real_t migration_rate, real_t propability, bool stick2boundary)
      : migration_rate_(migration_rate), propability_(propability), stick_to_boundary_(stick2boundary) {}

    virtual ~MyMigration() = default;

    void Initialize(const NewAgentEvent& event) override {
      // https://biodynamo.github.io/api/structbdm_1_1NewAgentEvent.html
      Base::Initialize(event);

      // if cell divides then behavior attributes have to be initialized
      if (auto* b = dynamic_cast<MyMigration*>(event.existing_behavior)) {
        migration_rate_ = b->GetMigrationRate();
        propability_ = b->GetPropability();
        stick_to_boundary_ = b->GetStickToBoundary();
      } else {
        Log::Fatal("MyMigration::Initialize",
                   "event.existing_behavior was not of type MyMigration");
      }
    }

    void Run(Agent* agent) override {
      auto* rand = Simulation::GetActive()->GetRandom();
      auto* param = Simulation::GetActive()->GetParam();

      if (auto* cell = dynamic_cast<Cell*>(agent)) {
        // check if a uniform random number is below the propability
        // parameter set to indicate the cell can migrate
        if (rand->Uniform() <= this->GetPropability()) {
          // calculate the cell (random) displacement after
          // multiplying the velocity with the simulation
          // time increment (time-step)
          real_t delta = this->GetMigrationRate() * param->simulation_time_step;
          Real3 displacement = rand->UniformArray<3>(-delta, +delta);
          // update the spatial location of the cell
          cell->UpdatePosition(displacement);

          Real3 xyz = cell->GetPosition();
          const real_t min_b = param->min_bound + 0.55 * cell->GetDiameter();
          const real_t max_b = param->max_bound - 0.55 * cell->GetDiameter();
          bool cell_on_bound = false;
          // perform the following checks to verify if the cell is
          // positioned approximately close to the simulation domain
          // boundaries (within some "margins" as indicated right above)
          if (xyz[0] < min_b) {
            xyz[0] = min_b;
            cell_on_bound = true;
          } else if (xyz[0] > max_b) {
            xyz[0] = max_b;
            cell_on_bound = true;
          }
          if (xyz[1] < min_b) {
            xyz[1] = min_b;
            cell_on_bound = true;
          } else if (xyz[1] > max_b) {
            xyz[1] = max_b;
            cell_on_bound = true;
          }
          if (xyz[2] < min_b) {
            xyz[2] = min_b;
            cell_on_bound = true;
          } else if (xyz[2] > max_b) {
            xyz[2] = max_b;
            cell_on_bound = true;
          }

          // https://biodynamo.github.io/api/classbdm_1_1Cell.html
          if (cell_on_bound) {
            // update the spatial position of the cell
            cell->SetPosition(xyz);
            // check if the flag 'stick_to_boundary_' indicates for the
            // cell to remain on the boundary of the simulation domain
            // forever or not
            if (this->GetStickToBoundary()) {
              // in this case then simply freeze the cell on the boundary
              // and never let it do anything else
              cell->RemoveBehavior(this);

              // NOTE: not a good strategy to provide model parameter values
              //       nested in the code; makes control of these parameters
              //       a great challenge
              real_t max_diameter = 4.0;
              real_t volume_growth_rate = 0.1;
              // check the 'my_growth.h' header file
              cell->AddBehavior(new MyGrowth(max_diameter, volume_growth_rate));
            }
          }
        }
      } else {
        Log::Fatal("MyMigration::Run", "Agent is not a Cell");
      }
    }

    real_t GetMigrationRate() const { return migration_rate_; }
    real_t GetPropability() const { return propability_; }
    bool GetStickToBoundary() const { return stick_to_boundary_; }

  private:
    real_t migration_rate_ = 1.0;
    real_t propability_ = 1.000;
    bool stick_to_boundary_ = false;
};

} // namespace bdm

#endif // MY_MIGRATION_H_
