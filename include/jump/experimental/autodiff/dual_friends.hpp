// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_DUAL_FRIENDS_HPP
#define JUMP_DUAL_FRIENDS_HPP

/// \relates Dual
friend Dual operator+(Dual lhs, const Dual& rhs) {
    lhs += rhs;
    return lhs;
}

/// \relates Dual
friend Dual operator+(const Dual& lhs, Dual&& rhs) {
    rhs += lhs;
    return rhs;
}

/// \relates Dual
friend Dual operator-(Dual lhs, const Dual& rhs) {
    lhs -= rhs;
    return lhs;
}

/// \relates Dual
friend Dual operator-(const Dual& lhs, Dual&& rhs) {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Dual
friend Dual operator*(Dual lhs, const Dual& rhs) {
    lhs *= rhs;
    return lhs;
}

/// \relates Dual
friend Dual operator*(const Dual& lhs, Dual&& rhs) {
    rhs *= lhs;
    return rhs;
}

/// \relates Dual
friend Dual operator/(Dual lhs, const Dual& rhs) {
    lhs /= rhs;
    return lhs;
}

// ========================================================================
// Exponentiation
// ========================================================================

/// \relates Dual
friend Dual exp(Dual x) {
    x.value = std::exp(x.value);
    for (auto& d : x.dual) {
        d *= x.value;
    }
    return x;
}

/// \relates Dual
friend Dual log(Dual x) {
    for (auto& d : x.dual) {
        d /= x.value;
    }
    x.value = std::log(x.value);
    return x;
}

/// \relates Dual
friend Dual pow(Dual x, Dual p) {
    return exp(log(x)*p);
}

// ========================================================================
// Trigonometry
// ========================================================================

/// \relates Dual
friend Dual sin(Dual x) {
    auto derivative{std::cos(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::sin(x.value);
    return x;
}

/// \relates Dual
friend Dual cos(Dual x) {
    auto derivative{-std::sin(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::cos(x.value);
    return x;
}

/// \relates Dual
friend Dual tan(Dual x) {
    auto derivative{1./(std::pow(std::cos(x.value), 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::tan(x.value);
    return  x;
}

/// \relates Dual
friend Dual asin(Dual x) {
    auto derivative{1./std::sqrt(1. - std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::asin(x.value);
    return x;
}

/// \relates Dual
friend Dual acos(Dual x) {
    auto derivative{-1./std::sqrt(1. - std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::acos(x.value);
    return x;
}

/// \relates Dual
friend Dual atan(Dual x) {
    auto derivative{1./(1. + std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::atan(x.value);
    return x;
}

// ========================================================================
// Hyperbolics
// ========================================================================

/// \relates Dual
friend Dual sinh(Dual x) {
    auto derivative{std::cosh(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::sinh(x.value);
    return x;
}

/// \relates Dual
friend Dual cosh(Dual x) {
    auto derivative{std::sinh(x.value)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::cosh(x.value);
    return x;
}

/// \relates Dual
friend Dual tanh(Dual x) {
    auto derivative{1./(std::pow(std::cosh(x.value), 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::tanh(x.value);
    return  x;
}

/// \relates Dual
friend Dual asinh(Dual x) {
    auto derivative{1./std::sqrt(1 + std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::asinh(x.value);
    return x;
}

/// \relates Dual
friend Dual acosh(Dual x) {
    auto derivative{1./std::sqrt(std::pow(x.value, 2.) - 1.)};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::acosh(x.value);
    return x;
}

/// \relates Dual
friend Dual atanh(Dual x) {
    auto derivative{1./(1 - std::pow(x.value, 2.))};
    for (auto& d : x.dual) {
        d *= derivative;
    }
    x.value = std::atanh(x.value);
    return x;
}

#endif  // JUMP_DUAL_FRIENDS_HPP

