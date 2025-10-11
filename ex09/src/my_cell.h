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
#ifndef MY_CELL_H_
#define MY_CELL_H_

namespace bdm {

class MyCell : public Cell {
  BDM_AGENT_HEADER(MyCell, Cell, 1);

  public:
    MyCell() {}
    explicit MyCell(const Real3& position) : Base(position) {}
    virtual ~MyCell() {}

    void Initialize(const NewAgentEvent& event) override {
      Base::Initialize(event);

      if (auto* mother = dynamic_cast<MyCell*>(event.existing_agent)) {
        if (event.GetUid() == CellDivisionEvent::kUid) {
          // copy properties from mother to daughter
          phenotype_ = mother->phenotype_;
        }
      }
    }

    int GetPhenotype() const { return phenotype_; }

    void SetPhenotype(int phenotype) { phenotype_ = phenotype; }

  private:
    int phenotype_ = 1;
};

} // namespace bdm

#endif // MY_CELL_H_
