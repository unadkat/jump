// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

/// Note that T must be appropriate for the chosen distribution, i.e. an
/// integral type for `std::uniform_int_distribution`, etc.
template <typename T, template <typename> typename Distribution>
template <typename... Args>
inline RandomNumbers<T, Distribution>::RandomNumbers(Args&&... args) :
    m_engine {std::random_device{}()},
    m_distribution {std::forward<Args>(args)...} {
}

template <typename T, template <typename> typename Distribution>
inline T RandomNumbers<T, Distribution>::generate() {
    return m_distribution(m_engine);
}

