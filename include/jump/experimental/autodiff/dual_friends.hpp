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

/// \relates Dual
friend Dual pow(Dual lhs, Real p) {
    for (std::size_t i{0}; i < N; ++i) {
        lhs.dual[i] *= p*std::pow(lhs.value, p - 1.);
    }
    lhs.value = std::pow(lhs.value, p);
    return lhs;
}

#endif  // JUMP_DUAL_FRIENDS_HPP

