#pragma once

#include "mathutils.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <algorithm>

template<class T, size_t N>
class MathArray {

public:
    T data[N];

    constexpr T& operator[](size_t i) noexcept {
        return this->data[i];
    }

    constexpr const T& operator[](size_t i) const noexcept {
        return this->data[i];
    }

    T& at(size_t i) {
        if (i >= N) {
            throw std::range_error("index out of range");
        }

        return this->data[i];
    }

    const T& at(size_t i) const {
        if (i >= N) {
            throw std::range_error("index out of range");
        }

        return this->data[i];
    }

    constexpr T* begin() noexcept {
        return std::begin(this->data);
    }

    constexpr T* end() noexcept {
        return std::end(this->data);
    }

    constexpr const T* begin() const noexcept {
        return std::begin(this->data);
    }

    constexpr const T* end() const noexcept {
        return std::end(this->data);
    }

    constexpr T& front() noexcept {
        static_assert(N > 0, "No data in 0-length array");

        return this->data[0];
    }

    constexpr T& back() noexcept {
        static_assert(N > 0, "No data in 0-length array");

        return this->data[N - 1];
    }

    constexpr const T& front() const noexcept {
        static_assert(N > 0, "No data in 0-length array");

        return this->data[0];
    }

    constexpr const T& back() const noexcept {
        static_assert(N > 0, "No data in 0-length array");

        return this->data[N - 1];
    }

    constexpr size_t size() const noexcept {
        return N;
    }

    constexpr size_t max_size() const noexcept {
        return N;
    }

    constexpr bool empty() const noexcept {
        return N == 0;
    }

    constexpr T sum() const noexcept {
        T output = 0;
        for (const T& t : *this) {
            output += t;
        }

        return output;
    }

    constexpr MathArray<T, N> cumsum() const noexcept {
        MathArray<T, N> output{};

        T running_total = 0;
        for (size_t i = 0; i < N; ++i) {
            running_total += (*this)[i];
            output[i] = running_total;
        }

        return output;
    }

    constexpr T prod() const noexcept {
        T output = 1;

        for (const T& t : *this) {
            output *= t;
        }

        return output;
    }

    constexpr MathArray<T, N> cumprod() const noexcept {
        MathArray<T, N> output{};

        T running_total = 1;
        for (size_t i = 0; i < N; ++i) {
            running_total *= (*this)[i];
            output[i] = running_total;
        }

        return output;
    }

    constexpr MathArray<T, N>& set(const size_t index, const T&& value) noexcept {
        this->data[index] = value;

        return *this;
    }

    constexpr MathArray<T, N>& set(const size_t index, const T& value) noexcept {
        this->data[index] = value;

        return *this;
    }

    constexpr MathArray<T, N> copy_set(const size_t index, const T& value) const noexcept {
        MathArray<T, N> output(*this);
        return output.set(index, value);
    }

    constexpr MathArray<T, N> copy_set(const size_t index, const T&& value) const noexcept {
        MathArray<T, N> output(*this);
        return output.set(index, value);
    }

    constexpr MathArray<T, N>& add_index(const size_t index, const T&& value) noexcept {
        this->data[index] += value;

        return *this;
    }

    constexpr MathArray<T, N>& add_index(const size_t index, const T& value) noexcept {
        this->data[index] += value;

        return *this;
    }

    constexpr MathArray<T, N> copy_add_index(const size_t index, const T& value) const noexcept {
        MathArray<T, N> output(*this);
        return output.add_index(index, value);
    }

    constexpr MathArray<T, N> copy_add_index(const size_t index, const T&& value) const noexcept {
        MathArray<T, N> output(*this);
        return output.add_index(index, value);
    }

    constexpr T dot(const MathArray<T, N>& other) const noexcept {
        return ((*this) * other).sum();
    }

    constexpr MathArray<T, N> cross(const MathArray<T, N>& other) const noexcept {
        static_assert(N == 3, "Can only cross 3-vectors");

        MathArray<T, N> output{};

        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                for (size_t k = 0; k < N; ++k) {
                    output[i] += levicevita(i, j, k) * (*this)[j] * other[k];
                }
            }
        }

        return output;
    }

    template <class U>
    constexpr MathArray<U, N> astype() const noexcept {
        MathArray<U, N> output{};

        for (size_t i = 0; i < N; ++i) {
            output[i] = (U) (*this)[i];
        }

        return output;
    }

    constexpr bool contains(const T& other) const noexcept {
        for (const auto v : *this) {
            if (other == v) {
                return true;
            }
        }

        return false;
    }

    constexpr void to_array(T (&arr)[N]) const noexcept {
        std::copy(std::begin(this->data), std::end(this->data), std::begin(arr));
    }
};


template <class T, size_t N>
constexpr MathArray<T, N> matharray_from_array(const T (&arr)[N]) noexcept {
    MathArray<T, N> out{};

    std::copy(std::begin(arr), std::end(arr), std::begin(out.data));

    return out;
}


template<class T, size_t N>
inline std::ostream& operator<< (std::ostream& out, const MathArray<T, N>& v) {
    if constexpr (N == 0) {
        out << "()";
        return out;
    } else {
        out << "(";

        for (size_t i = 0; i < N - 1; ++i) {
            out << v[i] << ", ";
        }

        out << v[N - 1] << ")";

        return out;
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ARITHMETIC OPERATIONS * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T, size_t N>
inline constexpr MathArray<T, N> operator+ (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] + a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+ (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] + a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+ (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 + a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] - a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] - a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const T& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1 - a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator* (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] * a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator* (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] * a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator* (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 * a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/ (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] / a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/ (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] / a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/ (const T& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1 / a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+= (MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] += a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+= (MathArray<T, N>& a1, const T& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] += a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator-= (MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] -= a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator-= (MathArray<T, N>& a1, const T& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] -= a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator*= (MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] *= a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator*= (MathArray<T, N>& a1, const T& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] *= a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/= (MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] /= a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/= (MathArray<T, N>& a1, const T& a2) noexcept {
    for (size_t i = 0; i < N; ++i) {
        a1[i] /= a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const MathArray<T, N>& a1) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = -a1[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr double magnitude_sq(const MathArray<T, N>& v) noexcept {
    double total = 0;

    for (size_t i = 0; i < N; ++i) {
        total += (v[i] * v[i]);
    }

    return total;
}

template<class T, size_t N>
inline constexpr double magnitude(const MathArray<T, N>& v) noexcept {
    return std::sqrt(magnitude_sq<T, N>(v));
}

template<class T, size_t N>
inline constexpr double distance_between_sq(const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    return magnitude_sq(a1 - a2);
}

template<class T, size_t N>
inline constexpr double distance_between(const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    return magnitude(a1 - a2);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * COMPARISON OPERATIONS * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T, size_t N>
inline constexpr MathArray<bool, N> operator< (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] < a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator< (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] < a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator> (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] > a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator> (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] > a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator> (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 < a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator<= (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] <= a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator< (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 > a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator<= (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] <= a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator>= (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] >= a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator>= (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] >= a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator<= (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 >= a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator>= (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 <= a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator== (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] == a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator== (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] == a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator== (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 == a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator!= (const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] != a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator!= (const MathArray<T, N>& a1, const T& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] != a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator!= (const T& a1, const MathArray<T, N>& a2) noexcept {
    return a2 != a1;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * BOOLEAN OPERATIONS  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<size_t N>
inline constexpr MathArray<bool, N> operator&& (const MathArray<bool, N>& a1, const MathArray<bool, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] && a2[i]);
    }

    return output;
}

template<size_t N>
inline constexpr MathArray<bool, N> operator|| (const MathArray<bool, N>& a1, const MathArray<bool, N>& a2) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] || a2[i]);
    }

    return output;
}

template<size_t N>
inline constexpr MathArray<bool, N> operator! (const MathArray<bool, N>& a1) noexcept {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = !a1[i];
    }

    return output;
}

template<size_t N>
inline constexpr bool all(const MathArray<bool, N>& a1) noexcept {
    for (const bool b : a1) {
        if (!b) {
            return false;
        }
    }

    return true;
}

template<size_t N>
inline constexpr bool any(const MathArray<bool, N>& a1) noexcept {
    for (const bool b : a1) {
        if (b) {
            return true;
        }
    }

    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * MORE COMPLICATED FUNCTIONS  * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T, size_t N>
inline constexpr MathArray<T, N> pow(const MathArray<T, N>& a1, const T& exponent) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = std::pow(a1[i], exponent);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> abs(const MathArray<T, N>& a1) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = std::abs(a1[i]);
    }

    return output;
}
