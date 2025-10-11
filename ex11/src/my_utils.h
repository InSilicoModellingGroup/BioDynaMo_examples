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
#ifndef MY_UTILS_H_
#define MY_UTILS_H_

#include "biodynamo.h"
#include "core/environment/uniform_grid_environment.h"
#include "core/scheduler.h"

namespace bdm {

inline real_t pow2(const real_t& a) { return (a * a); }
inline real_t pow3(const real_t& a) { return (pow2(a) * a); }
inline real_t pow4(const real_t& a) { return pow2(pow2(a)); }
inline real_t pow5(const real_t& a) { return (pow4(a) * a); }

inline real_t SquaredDistance(const Real3& u, const Real3& v) {
  const real_t dx = v[0] - u[0];
  const real_t dy = v[1] - u[1];
  const real_t dz = v[2] - u[2];
  return (pow2(dx) + pow2(dy) + pow2(dz));
}

}  // namespace bdm

#endif // MY_UTILS_H_
