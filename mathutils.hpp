#pragma once

#include <array>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <string>
#include <cstring>


inline constexpr int levicevita(const int i, const int j, const int k) {
    return (i - j) * (j - k) * (k - i) / 2;
}


inline constexpr int delta(const int i, const int j) {
    return int(i == j);
}

inline constexpr std::array<size_t, 2> unflatten_bin_index(const size_t index, const size_t bins_x, const size_t bins_y) {
    return std::array<size_t, 2>{
        index / bins_y, // x index
        index % bins_y  // y index
    };
}

inline constexpr size_t flatten_bin_index(const size_t index_x, const size_t index_y, const size_t bins_x, const size_t bins_y) {
    return index_x * bins_y + index_y;
}

template <class T>
inline constexpr T square(const T x) {
    return x * x;
}

template <class T>
inline constexpr T cube(const T x) {
    return x * x * x;
}

template <class T, int N>
inline constexpr T power(const T x) {
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
            throw number_format_exception("Unknown error occured in int conversion");
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
            throw number_format_exception("Unknown error occured in int conversion");
        }
    }

    return value;
}
