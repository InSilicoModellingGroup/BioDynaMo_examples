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
#ifndef MY_GROWTH_DIVISION_H_
#define MY_GROWTH_DIVISION_H_

#include "core/behavior/behavior.h"
#include "my_migration.h"

namespace bdm {

class MyGrowthDivision : public Behavior {
  BDM_BEHAVIOR_HEADER(MyGrowthDivision, Behavior, 1);

  public:
    MyGrowthDivision() { AlwaysCopyToNew(); }
    MyGrowthDivision(real_t threshold, real_t growth_rate, real_t propability)
      : threshold_(threshold), growth_rate_(growth_rate), propability_(propability) {}

    virtual ~MyGrowthDivision() = default;

    void Initialize(const NewAgentEvent& event) override {
      // https://biodynamo.github.io/api/structbdm_1_1NewAgentEvent.html
      Base::Initialize(event);

      // if cell divides then behavior attributes have to be initialized
      if (auto* b = dynamic_cast<MyGrowthDivision*>(event.existing_behavior)) {
        threshold_ = b->GetThreshold();
        growth_rate_ = b->GetGrowthRate();
        propability_ = b->GetPropability();
      } else {
        Log::Fatal("MyGrowthDivision::Initialize",
                   "event.existing_behavior was not of type MyGrowthDivision");
      }
    }

    void Run(Agent* agent) override {
      auto* rand = Simulation::GetActive()->GetRandom();

      if (auto* cell = dynamic_cast<Cell*>(agent)) {
        if (cell->GetDiameter() <= this->GetThreshold()) {
          cell->ChangeVolume(this->GetGrowthRate());
        } else if (rand->Uniform() <= this->GetPropability()) {
          auto* new_cell = cell->Divide();
          // https://biodynamo.github.io/api/classbdm_1_1Agent.html#ac6ff7e2073bd2b3e4794bc8f0a8c26ed
          auto behaviors = cell->GetAllBehaviors();
          for (uint16_t i = 0; i < behaviors.size(); ++i) {
            const Behavior* b = behaviors[i];
            if (auto* b = dynamic_cast<MyGrowthDivision*>(behaviors[i])) {
              // check what happens if the following line is commented:
              new_cell->AddBehavior(new MyGrowthDivision(b->GetThreshold(), b->GetGrowthRate(), b->GetPropability()));
            } else if (auto* b = dynamic_cast<MyMigration*>(behaviors[i])) {
              // check what happens if the following line is commented:
              new_cell->AddBehavior(new MyMigration(b->GetMigrationRate(), b->GetPropability()));
            }
          }
          // check what happens if the following line is commented:
        }
      } else {
        Log::Fatal("MyGrowthDivision::Run", "Agent is not a Cell");
      }
    }

    real_t GetThreshold() const { return threshold_; }
    real_t GetGrowthRate() const { return growth_rate_; }
    real_t GetPropability() const { return propability_; }

  private:
    real_t threshold_ = 10.0;
    real_t growth_rate_ = 1.0;
    real_t propability_ = 1.000;
};

}  // namespace bdm

#endif  // MY_GROWTH_DIVISION_H_
