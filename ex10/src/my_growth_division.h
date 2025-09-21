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
    MyGrowthDivision(real_t threshold, real_t growth_rate, real_t propability, real_t min_dist, real_t safe)
      : threshold_(threshold), growth_rate_(growth_rate), propability_(propability), smallest_distance_(min_dist), safe_distance_(safe) {}

    virtual ~MyGrowthDivision() = default;

    void Initialize(const NewAgentEvent& event) override {
      // https://biodynamo.github.io/api/structbdm_1_1NewAgentEvent.html
      Base::Initialize(event);

      // if cell divides then behavior attributes have to be initialized
      if (auto* b = dynamic_cast<MyGrowthDivision*>(event.existing_behavior)) {
        threshold_ = b->GetThreshold();
        growth_rate_ = b->GetGrowthRate();
        smallest_distance_ = b->GetSmallestDistance();
      } else {
        Log::Fatal("MyGrowthDivision::Initialize",
                   "event.existing_behavior was not of type MyGrowthDivision");
      }
    }

    void Run(Agent* agent) override {
      auto* rand = Simulation::GetActive()->GetRandom();
      auto* ctxt = Simulation::GetActive()->GetExecutionContext();

      if (auto* cell = dynamic_cast<MyCell*>(agent)) {
        AgentPointer<MyCell> other_cell = nullptr;

        // define how you'd like to do the cell search
        auto search_functor_ = L2F([&](Agent* agent_,
                                       real_t squared_distance_)
        {
          if (auto* neighbor_cell = dynamic_cast<const MyCell*>(agent_)) {
            if (cell->GetPhenotype() != neighbor_cell->GetPhenotype()) {
              real_t d2 = SquaredDistance(cell->GetPosition(),
                                          neighbor_cell->GetPosition());
              // check if cells are close enough
              if (d2 < pow2(this->GetSmallestDistance())) {
                other_cell =
                    AgentPointer<MyCell>(neighbor_cell->GetUid());
              }
            }
          }
        });

        if (safe_distance_>0.0) {
          // execute the search process
          ctxt->ForEachNeighbor(search_functor_, *cell, pow2(safe_distance_));
          if (other_cell != nullptr) {
            cell->RemoveBehavior(this);
            return;
          }
        }

        if (cell->GetDiameter() <= this->GetThreshold()) {
          cell->ChangeVolume(this->GetGrowthRate());
        } else if (rand->Uniform() <= this->GetPropability()) {
          auto* new_cell = cell->Divide();
          // check what happens if the following line is commented:
          new_cell->AddBehavior(new MyGrowthDivision(threshold_, growth_rate_, propability_, smallest_distance_, safe_distance_));
        }
      } else {
        Log::Fatal("MyGrowthDivision::Run", "Agent is not a MyCell");
      }
    }

    real_t GetThreshold() const { return threshold_; }
    real_t GetGrowthRate() const { return growth_rate_; }
    real_t GetPropability() const { return propability_; }
    real_t GetSmallestDistance() const { return smallest_distance_; }

  private:
    real_t threshold_ = 10.0;
    real_t growth_rate_ = 1.0;
    real_t propability_ = 1.000;
    real_t smallest_distance_ = 1.0;
    real_t safe_distance_ = 0.0;
};

}  // namespace bdm

#endif  // MY_GROWTH_DIVISION_H_
