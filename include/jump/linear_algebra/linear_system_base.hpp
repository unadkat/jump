// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_LINEAR_SYSTEM_BASE_HPP
#define JUMP_LINEAR_SYSTEM_BASE_HPP

#include <cstddef>

#include "../debug/exception.hpp"

namespace Jump {
/// \brief Interface for linear system solvers.
class LinearSystemBase
{
    public:
        /// \brief Solve the linear system.
        virtual void solve() = 0;
        /// \brief Return the number of equations in the linear system.
        virtual std::size_t order() const = 0;
};
}   // namespace Jump

#endif  // JUMP_LINEAR_SYSTEM_BASE

