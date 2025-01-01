// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_RANDOM_HPP
#define JUMP_RANDOM_HPP

#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/vector.hpp"

#include <complex>
#include <random>
#include <utility>

namespace jump {
/// \brief Supplies easy random number generation capabilities, with a templated
/// type for the random numbers and underlying distribution.
template <typename T, template <typename> typename Distribution>
class RandomNumbers {
    public:
        /// \brief Seeds the default random engine that is later used to
        /// generate random numbers from the templated distribution, forwarding
        /// whatever arguments are needed.
        template <typename... Args>
        RandomNumbers(Args&&... args);

        /// \brief Generate a random number of the templated type and
        /// distribution.
        auto generate() -> T;

    private:
        /// \brief The engine that generates the random seed data which is in
        /// turn used to provide numbers from a chosen distribution.
        std::default_random_engine m_engine;
        Distribution<T> m_distribution;
};

template <typename Rng, typename T>
void randomise(Rng& rng, T& item);

template <typename Rng, typename... Ts>
void randomise(Rng& rng, Ts& ...items);

using RandomReal = RandomNumbers<double, std::uniform_real_distribution>;
using RandomInt = RandomNumbers<long, std::uniform_int_distribution>;

// ========================================================================
// Implementation
// ========================================================================

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

