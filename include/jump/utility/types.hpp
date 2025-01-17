// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_TYPES_HPP
#define JUMP_TYPES_HPP

#include <complex>

namespace jump {
using Real = double;
using Complex = std::complex<double>;

template <typename>
struct is_complex : public std::false_type {
};

template <typename T>
struct is_complex<std::complex<T>> : public std::true_type {
};

template <typename T>
inline constexpr bool is_complex_v = is_complex<T>::value;
}   // namespace jump

#endif  // JUMP_TYPES_HPP

