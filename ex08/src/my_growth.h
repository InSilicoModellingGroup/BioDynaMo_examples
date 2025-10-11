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
#ifndef MY_GROWTH_H_
#define MY_GROWTH_H_

#include "core/behavior/behavior.h"

namespace bdm {

class MyGrowth : public Behavior {
  BDM_BEHAVIOR_HEADER(MyGrowth, Behavior, 1);

  public:
    MyGrowth() { AlwaysCopyToNew(); }
    MyGrowth(real_t threshold, real_t growth_rate)
      : threshold_(threshold), growth_rate_(growth_rate) {}

    virtual ~MyGrowth() = default;

    void Initialize(const NewAgentEvent& event) override {
      // https://biodynamo.github.io/api/structbdm_1_1NewAgentEvent.html
      Base::Initialize(event);

      // if cell divides then behavior attributes have to be initialized
      if (auto* b = dynamic_cast<MyGrowth*>(event.existing_behavior)) {
        threshold_ = b->GetThreshold();
        growth_rate_ = b->GetGrowthRate();
      } else {
        Log::Fatal("MyGrowth::Initialize",
                   "event.existing_behavior was not of type MyGrowth");
      }
    }

    void Run(Agent* agent) override {
      if (auto* cell = dynamic_cast<Cell*>(agent)) {
        // check if cell diameter is below a fixed threshold value
        if (cell->GetDiameter() <= this->GetThreshold()) {
          // now increase the cell volume provided the (constant)
          // speed by which its size increases
          cell->ChangeVolume(this->GetGrowthRate());
        }
      } else {
        Log::Fatal("MyGrowth::Run", "Agent is not a Cell");
      }
    }

    real_t GetThreshold() const { return threshold_; }
    real_t GetGrowthRate() const { return growth_rate_; }

  private:
    real_t threshold_ = 10.0;
    real_t growth_rate_ = 1.0;
};

} // namespace bdm

#endif // MY_GROWTH_H_
