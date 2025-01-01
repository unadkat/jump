// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_DUAL_FRIENDS_HPP
#define JUMP_DUAL_FRIENDS_HPP

/// \relates Dual
friend auto operator+(Dual lhs, const Dual& rhs) -> Dual {
    lhs += rhs;
    return lhs;
}

/// \relates Dual
friend auto operator+(const Dual& lhs, Dual&& rhs) -> Dual {
    rhs += lhs;
    return rhs;
}

/// \relates Dual
friend auto operator-(Dual lhs, const Dual& rhs) -> Dual {
    lhs -= rhs;
    return lhs;
}

/// \relates Dual
friend auto operator-(const Dual& lhs, Dual&& rhs) -> Dual {
    rhs *= T{-1};
    rhs += lhs;
    return rhs;
}

/// \relates Dual
friend auto operator*(Dual lhs, const Dual& rhs) -> Dual {
    lhs *= rhs;
    return lhs;
}

/// \relates Dual
friend auto operator*(const Dual& lhs, Dual&& rhs) -> Dual {
    rhs *= lhs;
    return rhs;
}

/// \relates Dual
friend auto operator/(Dual lhs, const Dual& rhs) -> Dual {
    lhs /= rhs;
    return lhs;
}

#endif  // JUMP_DUAL_FRIENDS_HPP

