// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_RANDOM_HPP
#define JUMP_RANDOM_HPP

#include "jump/utility/random_decl.hpp"

#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/vector.hpp"

#include <complex>
#include <utility>

namespace jump {
/// Note that T must be appropriate for the chosen distribution, i.e. an
/// integral type for `std::uniform_int_distribution`, etc.
template <typename T, template <typename> typename Distribution>
template <typename... Args>
inline RandomNumbers<T, Distribution>::RandomNumbers(Args&&... args) :
    m_engine{std::random_device{}()},
    m_distribution{std::forward<Args>(args)...} {
}

template <typename T, template <typename> typename Distribution>
inline auto RandomNumbers<T, Distribution>::generate() -> T {
    return m_distribution(m_engine);
}

template <typename Rng, typename T>
inline void randomise(Rng& rng, T& item) {
    item = rng.generate();
}

template <typename Rng, typename... Ts>
inline void randomise(Rng& rng, Ts& ...items) {
    (..., randomise(rng, items));
}

template <typename Rng, typename T>
inline void randomise(Rng& rng, std::complex<T>& item) {
    item = {T{rng.generate()}, T{rng.generate()}};
}

template <typename Rng, typename T>
inline void randomise(Rng& rng, Vector<T>& container) {
    for (auto& x : container.storage) {
        randomise(rng, x);
    }
}

template <typename Rng, typename T>
inline void randomise(Rng& rng, BandedMatrix<T>& container) {
    Vector<T> values(container.num_elements());
    randomise(rng, values);
    container.assign(std::move(values));
}

template <typename Rng, typename T>
inline void randomise(Rng& rng, DenseMatrix<T>& container) {
    Vector<T> values(container.num_elements());
    randomise(rng, values);
    container.assign(std::move(values));
}
}   // namespace jump

#endif  // JUMP_RANDOM_HPP

