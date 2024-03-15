// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_RANDOM_DECL_HPP
#define JUMP_RANDOM_DECL_HPP

#include <random>

#include "jump/data/banded_matrix.hpp"
#include "jump/data/dense_matrix.hpp"
#include "jump/data/vector.hpp"
#include "jump/utility/types.hpp"

namespace jump {
/// \brief Supplies easy random number generation capabilities, with a templated
/// type for the random numbers and underlying distribution.
template <typename T = Real, template <typename> typename Distribution
    = std::uniform_real_distribution>
class RandomNumbers {
    private:
        /// \brief The engine that generates the random seed data which is in
        /// turn used to provide numbers from a chosen distribution.
        std::default_random_engine m_engine;
        Distribution<T> m_distribution;

    public:
        /// \brief Seeds the default random engine that is later used to
        /// generate random numbers from the templated distribution, forwarding
        /// whatever arguments are needed.
        template <typename... Args>
        RandomNumbers(Args&&... args);

        /// \brief Generate a random number of the templated type and
        /// distribution.
        T generate();
};

template <typename Rng, typename T>
void randomise(Rng& rng, T& item);

template <typename Rng, typename... Ts>
void randomise(Rng& rng, Ts& ...items);

template <typename Rng, template <typename> typename Container, typename T>
void randomise(Rng& rng, Container<T>& container);
}   // namespace jump

#endif  // JUMP_RANDOM_DECL_HPP

