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
    MyMigration(real_t migration_rate, real_t propability)
      : migration_rate_(migration_rate), propability_(propability) {}

    virtual ~MyMigration() = default;

    void Initialize(const NewAgentEvent& event) override {
      // https://biodynamo.github.io/api/structbdm_1_1NewAgentEvent.html
      Base::Initialize(event);

      // if cell divides then behavior attributes have to be initialized
      if (auto* b = dynamic_cast<MyMigration*>(event.existing_behavior)) {
        migration_rate_ = b->GetMigrationRate();
        propability_ = b->GetPropability();
      } else {
        Log::Fatal("MyMigration::Initialize",
                   "event.existing_behavior was not of type MyMigration");
      }
    }

    void Run(Agent* agent) override {
      auto* rand = Simulation::GetActive()->GetRandom();
      auto* param = Simulation::GetActive()->GetParam();

      if (auto* cell = dynamic_cast<MyCell*>(agent)) {
        if (rand->Uniform() > this->GetPropability()) return;

        real_t delta = this->GetMigrationRate() * param->simulation_time_step;
        Real3 displacement = rand->UniformArray<3>(-delta, +delta);
        // https://biodynamo.github.io/api/classbdm_1_1Cell.html
        cell->UpdatePosition(displacement);

      } else {
        Log::Fatal("MyMigration::Run", "Agent is not a MyCell");
      }
    }

    real_t GetMigrationRate() const { return migration_rate_; }
    real_t GetPropability() const { return propability_; }

  private:
    real_t migration_rate_ = 1.0;
    real_t propability_ = 1.000;
};

}  // namespace bdm

#endif  // MY_MIGRATION_H_
