#pragma once

#include <array>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>
#include <random>


inline constexpr int levicevita(const int i, const int j, const int k) noexcept {
    return (i - j) * (j - k) * (k - i) / 2;
}


inline constexpr int delta(const int i, const int j) noexcept {
    return int(i == j);
}

inline constexpr std::array<size_t, 2> unflatten_bin_index(const size_t index, const size_t bins_x, const size_t bins_y) noexcept {
    return std::array<size_t, 2>{
        index / bins_y, // x index
        index % bins_y  // y index
    };
}

inline constexpr size_t flatten_bin_index(const size_t index_x, const size_t index_y, const size_t bins_x, const size_t bins_y) noexcept {
    return index_x * bins_y + index_y;
}

template <class T>
inline constexpr T square(const T x) noexcept {
    return x * x;
}

template <class T>
inline constexpr T cube(const T x) noexcept {
    return x * x * x;
}

inline constexpr double hypot(const double a, const double b) noexcept {
    return std::sqrt(square(a) + square(b));
}

template <class T, int N>
inline constexpr T power(const T x) noexcept {
    T out = 1;

    for (size_t i = 0; i < N; ++i) {
        out *= x;
    }

    return out;
}

struct number_format_exception : public std::runtime_error {

	number_format_exception(const char* message) : std::runtime_error(message) {}
    number_format_exception(const std::string& message) : std::runtime_error(message) {}

};

template <class T>
inline T convert(const std::string& s);

template <>
inline std::string convert(const std::string& s) {
    return s;
}

template <>
inline int convert(const std::string& s) {
    char* leftovers;
    const long value = std::strtol(s.c_str(), &leftovers, 10);

    if (value == 0) {
        if (errno == EINVAL) {
            throw number_format_exception("Couldn't convert string to int");
        }

        if (errno == ERANGE) {
            throw number_format_exception("Value too big for long");
        }

        if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
            throw number_format_exception("Value too big for int");
        }

        if (std::strcmp(leftovers, s.c_str()) == 0) {
            throw number_format_exception("Unknown error occured in int conversion");
        }
    }

    return value;
}

template <>
inline long convert(const std::string& s) {
    char* leftovers;
    const long value = std::strtol(s.c_str(), &leftovers, 10);

    if (value == 0) {
        if (errno == EINVAL) {
            throw number_format_exception("Couldn't convert string to int");
        }

        if (errno == ERANGE) {
            throw number_format_exception("Value too big for long");
        }

        if (std::strcmp(leftovers, s.c_str()) == 0) {
            throw number_format_exception("Unknown error occured in long conversion");
        }
    }

    return value;
}

template <>
inline double convert(const std::string& s) {
    char* leftovers;
    const double value = std::strtod(s.c_str(), &leftovers);

    if (value == 0) {
        if (errno == EINVAL) {
            throw number_format_exception("Couldn't convert string to double");
        }

        if (errno == ERANGE) {
            throw number_format_exception("Value too big for double");
        }

        if (std::strcmp(leftovers, s.c_str()) == 0) {
            throw number_format_exception("Unknown error occured in double conversion");
        }
    }

    return value;
}

template <class T>
inline T& choice(const std::vector<T>& v, std::mt19937& engine) noexcept {
    return v[std::uniform_int_distribution(0, v.size() - 1)(engine)];
}


template <class T>
inline std::vector<T> cumsum(const std::vector<T>& other) noexcept {
    std::vector<T> output(other.size());

    output[0] = other[0];
    for (size_t i = 1; i < other.size(); ++i) {
        output[i] = output[i - 1] + other[i];
    }

    return output;
}

/**
  * Alas, linear time rather than clever logarithmic time. Had to implement this
  * because very occasionally, std::lower_bound would return v.end() instead of
  * the correct value. I have no idea why - the vector was sorted and all... :(
  */
template <class T>
size_t find_first_element_greater_than(const std::vector<T>& v, const T& search) noexcept {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] > search) {
            return i;
        }
    }

    return v.size();
}

/**
  * Pick an index using the given weights. e.g. if weights is [1, 2, 0] then we
  * except to see '2' 0% of the time, '1' 67% of the time, and '0' 33% of the
  * time.
  */
template <class T>
inline size_t weighted_index(const std::vector<T>& weights, std::mt19937& engine) noexcept {
    const std::vector<T> cdf = cumsum(weights);
    const double rand = std::uniform_real_distribution<T>(0, cdf.back())(engine);
    const size_t index = find_first_element_greater_than(cdf, rand);

    return index;
}
