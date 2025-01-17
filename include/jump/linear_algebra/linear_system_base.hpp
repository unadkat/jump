// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_LINEAR_SYSTEM_BASE_HPP
#define JUMP_LINEAR_SYSTEM_BASE_HPP

#include <cstddef>

namespace jump {
/// \brief Interface for linear system solvers.
class LinearSystemBase
{
    public:
        /// \brief Solve the linear system.
        virtual void solve() = 0;
        /// \brief Return the number of equations in the linear system.
        virtual auto order() const -> std::size_t = 0;
};
}   // namespace jump

#endif  // JUMP_LINEAR_SYSTEM_BASE

