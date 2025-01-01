// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

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

