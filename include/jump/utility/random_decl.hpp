// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_RANDOM_DECL_HPP
#define JUMP_RANDOM_DECL_HPP

#include <random>

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
}   // namespace jump

#endif  // JUMP_RANDOM_DECL_HPP

