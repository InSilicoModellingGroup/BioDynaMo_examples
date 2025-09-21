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
        if (rand->Uniform() <= this->GetPropability()) {
          real_t delta = this->GetMigrationRate() * param->simulation_time_step;
          Real3 displacement = rand->UniformArray<3>(-delta, +delta);
          // https://biodynamo.github.io/api/classbdm_1_1Cell.html
          cell->UpdatePosition(displacement);

          Real3 xyz = cell->GetPosition();
          const real_t min_b = param->min_bound + 0.55 * cell->GetDiameter();
          const real_t max_b = param->max_bound - 0.55 * cell->GetDiameter();
          bool cell_on_bound = false;
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
            cell->SetPosition(xyz);
            if (this->GetStickToBoundary()) {
              cell->RemoveBehavior(this);
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

}  // namespace bdm

#endif  // MY_MIGRATION_H_
