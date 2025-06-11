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
      threshold_ = b->threshold_;
      growth_rate_ = b->growth_rate_;
      propability_ = b->propability_;
    } else {
      Log::Fatal("MyGrowthDivision::Initialize",
                 "event.existing_behavior was not of type MyGrowthDivision");
    }
  }

  void Run(Agent* agent) override {
    auto* rand = Simulation::GetActive()->GetRandom();

    if (auto* cell = dynamic_cast<Cell*>(agent)) {
      if (cell->GetDiameter() <= threshold_) {
        cell->ChangeVolume(growth_rate_);
      } else if (rand->Uniform() <= propability_) {
        auto* new_cell = cell->Divide();
        // check what happens if the following line is commented:
        new_cell->AddBehavior(new MyGrowthDivision(threshold_, growth_rate_, propability_));
      }
    } else {
      Log::Fatal("MyGrowthDivision::Run", "Agent is not a Cell");
    }
  }

  private:
    real_t threshold_ = 10.0;
    real_t growth_rate_ = 1.0;
    real_t propability_ = 1.000;
};

}  // namespace bdm

#endif  // MY_GROWTH_DIVISION_H_
