// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_VEC_HPP
#define JUMP_VEC_HPP

#include <ostream>

namespace jump {
template <typename T, std::size_t N>
struct Vec {
};

template <typename T, std::size_t N>
inline auto operator<<(std::ostream& out, const Vec<T, N>& rhs)
        -> std::ostream& {
    for (std::size_t i{0}; i < rhs.size(); ++i) {
        out << rhs[i] << ' ';
    }
    return out;
}
}   // namespace jump

#endif  // JUMP_VEC_HPP

