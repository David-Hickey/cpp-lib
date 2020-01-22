#pragma once

#include "mathutils.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iterator>

template<class T, size_t N>
class MathArray {

public:
    T data[N];

    T& operator[](size_t i) {
        return this->data[i];
    }

    const T& operator[](size_t i) const {
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

    T* begin() {
        return std::begin(this->data);
    }

    T* end() {
        return std::end(this->data);
    }

    const T* begin() const {
        return std::begin(this->data);
    }

    const T* end() const {
        return std::end(this->data);
    }

    T& front() {
        return this->data[0];
    }

    T& back() {
        return this->data[N - 1];
    }

    const T& front() const {
        return this->data[0];
    }

    const T& back() const {
        return this->data[N - 1];
    }

    size_t size() const {
        return N;
    }

    size_t max_size() const {
        return N;
    }

    bool empty() const {
        return N == 0;
    }

    T sum() const {
        T output = 0;
        for (const T& t : *this) {
            output += t;
        }

        return output;
    }

    MathArray<T, N> cumsum() const {
        MathArray<T, N> output{};

        T running_total = 0;
        for (size_t i = 0; i < N; ++i) {
            running_total += (*this)[i];
            output[i] = running_total;
        }

        return output;
    }

    T prod() const {
        T output = 1;

        for (const T& t : *this) {
            output *= t;
        }

        return output;
    }

    MathArray<T, N> cumprod() const {
        MathArray<T, N> output{};

        T running_total = 1;
        for (size_t i = 0; i < N; ++i) {
            running_total *= (*this)[i];
            output[i] = running_total;
        }

        return output;
    }

    MathArray<T, N>& set(const size_t index, const T&& value) {
        this->data[index] = value;

        return *this;
    }

    MathArray<T, N>& set(const size_t index, const T& value) {
        this->data[index] = value;

        return *this;
    }

    MathArray<T, N> copy_set(const size_t index, const T& value) const {
        MathArray<T, N> output(*this);
        return output.set(index, value);
    }

    MathArray<T, N> copy_set(const size_t index, const T&& value) const {
        MathArray<T, N> output(*this);
        return output.set(index, value);
    }

    MathArray<T, N>& add_index(const size_t index, const T&& value) {
        this->data[index] += value;

        return *this;
    }

    MathArray<T, N>& add_index(const size_t index, const T& value) {
        this->data[index] += value;

        return *this;
    }

    MathArray<T, N> copy_add_index(const size_t index, const T& value) const {
        MathArray<T, N> output(*this);
        return output.add_index(index, value);
    }

    MathArray<T, N> copy_add_index(const size_t index, const T&& value) const {
        MathArray<T, N> output(*this);
        return output.add_index(index, value);
    }

    T dot(const MathArray<T, N>& other) const {
        return ((*this) * other).sum();
    }

    MathArray<T, N> cross(const MathArray<T, N>& other) const {
        if (N == 3) {
            MathArray<T, N> output{};

            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    for (size_t k = 0; k < N; ++k) {
                        output[i] += levicevita(i, j, k) * (*this)[j] * other[k];
                    }
                }
            }

            return output;
        } else {
            return MathArray<T, N>{};
        }
    }

    template <class U>
    MathArray<U, N> astype() const {
        MathArray<U, N> output{};

        for (size_t i = 0; i < 3; ++i) {
            output[i] = (U) (*this)[i];
        }

        return output;
    }

    bool contains(const T& other) {
        for (const auto v : *this) {
            if (other == v) {
                return true;
            }
        }

        return false;
    }
};


template<class T, size_t N>
inline std::ostream& operator<< (std::ostream& out, const MathArray<T, N>& v) {
    out << "(";

    for (size_t i = 0; i < N - 1; ++i) {
        out << v[i] << ", ";
    }

    out << v[N - 1] << ")";

    return out;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ARITHMETIC OPERATIONS * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T, size_t N>
inline constexpr MathArray<T, N> operator+ (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] + a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+ (const MathArray<T, N>& a1, const T& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] + a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+ (const T& a1, const MathArray<T, N>& a2) {
    return a2 + a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] - a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const MathArray<T, N>& a1, const T& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] - a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const T& a1, const MathArray<T, N>& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1 - a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator* (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] * a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator* (const MathArray<T, N>& a1, const T& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] * a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator* (const T& a1, const MathArray<T, N>& a2) {
    return a2 * a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/ (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] / a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/ (const MathArray<T, N>& a1, const T& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] / a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/ (const T& a1, const MathArray<T, N>& a2) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1 / a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+= (MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] += a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator+= (MathArray<T, N>& a1, const T& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] += a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator-= (MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] -= a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator-= (MathArray<T, N>& a1, const T& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] -= a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator*= (MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] *= a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator*= (MathArray<T, N>& a1, const T& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] *= a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/= (MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] /= a2[i];
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator/= (MathArray<T, N>& a1, const T& a2) {
    for (size_t i = 0; i < N; ++i) {
        a1[i] /= a2;
    }

    return a1;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> operator- (const MathArray<T, N>& a1) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = -a1[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr double magnitude_sq(const MathArray<T, N>& v) {
    double total = 0;

    for (size_t i = 0; i < N; ++i) {
        total += (v[i] * v[i]);
    }

    return total;
}

template<class T, size_t N>
inline constexpr double magnitude(const MathArray<T, N>& v) {
    return std::sqrt(magnitude_sq<T, N>(v));
}

template<class T, size_t N>
inline constexpr double distance_between_sq(const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    return magnitude_sq(a1 - a2);
}

template<class T, size_t N>
inline constexpr double distance_between(const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    return magnitude(a1 - a2);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * COMPARISON OPERATIONS * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T, size_t N>
inline constexpr MathArray<bool, N> operator< (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] < a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator< (const MathArray<T, N>& a1, const T& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] < a2;
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator> (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = a1[i] > a2[i];
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator> (const MathArray<T, N>& a1, const T& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] > a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator> (const T& a1, const MathArray<T, N>& a2) {
    return a2 < a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator<= (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] <= a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator< (const T& a1, const MathArray<T, N>& a2) {
    return a2 > a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator<= (const MathArray<T, N>& a1, const T& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] <= a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator>= (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] >= a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator>= (const MathArray<T, N>& a1, const T& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] >= a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator<= (const T& a1, const MathArray<T, N>& a2) {
    return a2 >= a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator>= (const T& a1, const MathArray<T, N>& a2) {
    return a2 <= a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator== (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] == a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator== (const MathArray<T, N>& a1, const T& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] == a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator== (const T& a1, const MathArray<T, N>& a2) {
    return a2 == a1;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator!= (const MathArray<T, N>& a1, const MathArray<T, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] != a2[i]);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator!= (const MathArray<T, N>& a1, const T& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] != a2);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<bool, N> operator!= (const T& a1, const MathArray<T, N>& a2) {
    return a2 != a1;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * BOOLEAN OPERATIONS  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<size_t N>
inline constexpr MathArray<bool, N> operator&& (const MathArray<bool, N>& a1, const MathArray<bool, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] && a2[i]);
    }

    return output;
}

template<size_t N>
inline constexpr MathArray<bool, N> operator|| (const MathArray<bool, N>& a1, const MathArray<bool, N>& a2) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = (a1[i] || a2[i]);
    }

    return output;
}

template<size_t N>
inline constexpr MathArray<bool, N> operator! (const MathArray<bool, N>& a1) {
    MathArray<bool, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = !a1[i];
    }

    return output;
}

template<size_t N>
inline constexpr bool all(const MathArray<bool, N>& a1) {
    for (const bool b : a1) {
        if (!b) {
            return false;
        }
    }

    return true;
}

template<size_t N>
inline constexpr bool any(const MathArray<bool, N>& a1) {
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
inline constexpr MathArray<T, N> pow(const MathArray<T, N>& a1, const T& exponent) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = std::pow(a1[i], exponent);
    }

    return output;
}

template<class T, size_t N>
inline constexpr MathArray<T, N> abs(const MathArray<T, N>& a1) {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = std::abs(a1[i]);
    }

    return output;
}
