// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_RANDOM_DECL_HPP
#define JUMP_RANDOM_DECL_HPP

#include <random>

namespace jump {
/// \brief Supplies easy random number generation capabilities, with a templated
/// type for the random numbers and underlying distribution.
template <typename T = double, template <typename> typename Distribution
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
}   // namespace jump

#endif  // JUMP_RANDOM_DECL_HPP

