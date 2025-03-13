// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_VEC2_HPP
#define JUMP_VEC2_HPP

#include "jump/experimental/data/vec.hpp"

namespace jump {
template <typename T>
struct Vec<T, 2> {
    union {T x, r, s;};
    union {T y, g, t;};
};
}   // namespace jump

#endif  // JUMP_VEC2_HPP

