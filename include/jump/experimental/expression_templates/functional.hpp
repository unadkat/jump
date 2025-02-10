// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_EXPRESSION_TEMPLATE_FUNCTIONAL_HPP
#define JUMP_EXPRESSION_TEMPLATE_FUNCTIONAL_HPP

#include "jump/autodiff/dual.hpp"

#include <cmath>

namespace jump::functional {
struct Exp {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::exp;
        return exp(std::forward<T>(arg));
    }
};

struct Log {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::log;
        return log(std::forward<T>(arg));
    }
};
}   // namespace jump::functional

#endif  // JUMP_EXPRESSION_TEMPLATE_FUNCTIONAL_HPP

