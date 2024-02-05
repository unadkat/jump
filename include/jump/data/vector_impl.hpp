// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

/// \class Vector
/// Most operations are simply passed through to the underlying `std::vector`
/// storage, but arithmetic operations are implemented in two forms. Generally,
/// a simple default version for all basic `Vector` operations is provided, but
/// in the case of the preprocessor definition `HAS_CBLAS` we will offload to
/// a local cblas implementation.

template <typename T>
inline Vector<T>::Vector(const std::size_t& size, const T& value) :
    m_storage(size, value) {
}

template <typename T>
inline Vector<T>::Vector(std::initializer_list<T>&& list) :
    m_storage(std::forward<std::initializer_list<T>>(list)) {
}

/// Useful when extracting columns from a matrix, for example. Combined with
/// a way of getting iterators to the internal storage locations
/// representing the top and bottom of a given matrix column, this is the
/// fastest way of obtaining that isolated column.
template <typename T>
template <class InputIt>
inline Vector<T>::Vector(InputIt first, InputIt last) :
    m_storage(std::move(first), std::move(last)) {
}

/// \brief Conversion operator to promote a real-valued `Vector` to a
/// complex-valued one.
///
/// Initialises a complex-valued `Vector` of the correct size and loops
/// through it to convert the real-valued elements in the source `Vector`.
/// This is not terribly efficient but the only way of achieving the
/// promotion easily.
template <>
inline Vector<Real>::operator Vector<Complex>() const {
    Vector<Complex> result(m_storage.size());
    for (std::size_t i{0}; i < result.size(); ++i) {
        result[i] = m_storage[i];
    }
    return result;
}

template <typename T>
inline const T& Vector<T>::operator[](std::size_t index) const {
    assert(index < m_storage.size());
    return m_storage[index];
}

template <typename T>
inline T& Vector<T>::operator[](std::size_t index) {
    assert(index < m_storage.size());
    return m_storage[index];
}

template <typename T>
inline void Vector<T>::assign(std::size_t size, const T& value) {
    m_storage.assign(size, value);
}

template <typename T>
template <class InputIt>
inline void Vector<T>::assign(InputIt first, InputIt last) {
    m_storage.assign(std::move(first), std::move(last));
}

template <typename T>
inline void Vector<T>::reserve(std::size_t size) {
    m_storage.reserve(size);
}

template <typename T>
inline void Vector<T>::clear() {
    m_storage.clear();
}

template <typename T>
inline void Vector<T>::resize(std::size_t size) {
    m_storage.resize(size);
}

template <typename T>
inline std::size_t Vector<T>::size() const {
    return m_storage.size();
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::begin() const {
    return m_storage.begin();
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::end() const {
    return m_storage.end();
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::begin() {
    return m_storage.begin();
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::end() {
    return m_storage.end();
}

template <typename T>
inline void Vector<T>::fill(const T& value) {
    std::fill(m_storage.begin(), m_storage.end(), value);
}

template <typename T>
inline void Vector<T>::zero() {
    fill(T{0});
}

template <typename T>
inline const Vector<T>& Vector<T>::operator+() const {
    return *this;
}

template <typename T>
inline Vector<T> Vector<T>::operator-() const {
    Vector<T> temp{*this};
    return temp *= -1.;
}

template <typename T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs) {
    assert(rhs.size() == m_storage.size());
    for (std::size_t i{0}, size{m_storage.size()}; i < size; ++i) {
        m_storage[i] += rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs) {
    assert(rhs.size() != m_storage.size());
    for (std::size_t i{0}, size{m_storage.size()}; i < size; ++i) {
        m_storage[i] -= rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator*=(const T& rhs) {
    for (auto& x : m_storage) {
        x *= rhs;
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator/=(const T& rhs) {
    for (auto& x : m_storage) {
        x /= rhs;
    }
    return *this;
}

template <typename T>
inline Real Vector<T>::L1_norm() const {
    Real temp{0.};
    for (const auto& x : m_storage) {
        temp += std::abs(x);
    }
    return temp;
}

template <typename T>
inline Real Vector<T>::L2_norm() const {
    Real temp{0.};
    for (const auto& x : m_storage) {
        temp += std::pow(std::abs(x), 2.);
    }
    return std::sqrt(temp);
}

template <typename T>
inline Real Vector<T>::Linf_norm() const {
    Real temp{0.};
    for (auto&& x : m_storage) {
        if (temp < std::abs(x)) {
            temp = std::abs(x);
        }
    }
    return temp;
}

template <typename T>
inline T* Vector<T>::data_pointer() {
    return m_storage.data();
}

template <typename T>
inline const T* Vector<T>::data_pointer() const {
    return m_storage.data();
}

