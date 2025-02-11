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

// ========================================================================
// Trigonometry
// ========================================================================

struct Sin {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::sin;
        return sin(std::forward<T>(arg));
    }
};

struct Cos {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::cos;
        return cos(std::forward<T>(arg));
    }
};

struct Tan {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::tan;
        return tan(std::forward<T>(arg));
    }
};

struct Asin {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::asin;
        return asin(std::forward<T>(arg));
    }
};

struct Acos {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::acos;
        return acos(std::forward<T>(arg));
    }
};

struct Atan {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::atan;
        return atan(std::forward<T>(arg));
    }
};

// ========================================================================
// Hyperbolics
// ========================================================================

struct Sinh {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::sinh;
        return sinh(std::forward<T>(arg));
    }
};

struct Cosh {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::cosh;
        return cosh(std::forward<T>(arg));
    }
};

struct Tanh {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::tanh;
        return tanh(std::forward<T>(arg));
    }
};

struct Asinh {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::asinh;
        return asinh(std::forward<T>(arg));
    }
};

struct Acosh {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::acosh;
        return acosh(std::forward<T>(arg));
    }
};

struct Atanh {
    template <typename T>
    constexpr auto operator()(T&& arg) const {
        using std::atanh;
        return atanh(std::forward<T>(arg));
    }
};
}   // namespace jump::functional

#endif  // JUMP_EXPRESSION_TEMPLATE_FUNCTIONAL_HPP

