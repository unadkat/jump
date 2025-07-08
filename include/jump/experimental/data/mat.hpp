// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_MAT_HPP
#define JUMP_MAT_HPP

#include <ostream>

namespace jump {
template <typename T, std::size_t N>
struct Mat {
};

template <typename T, std::size_t N>
inline auto operator<<(std::ostream& out, const Mat<T, N>& rhs)
        -> std::ostream& {
    return out << rhs.as_string();
}
}   // namespace jump

#endif  // JUMP_MAT_HPP

