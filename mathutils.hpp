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
struct MilsteinParameters {
    T alpha_1;
    T alpha_0;

    T beta_1;
    T beta_0;

    T gamma_1;
    T gamma_0;

    MilsteinParameters(const T alpha_1, const T alpha_0, const T beta_1, const T beta_0)
    : alpha_1(alpha_1)
    , alpha_0(alpha_0)
    , beta_1(beta_1)
    , beta_0(beta_0)
    , gamma_1(1)
    , gamma_0(1 + alpha_1) {}
};

const static MilsteinParameters<double> adams_bashforth_parameters(
    -1.0, 0.0,
    1.5, -0.5
);

const static MilsteinParameters<double> midpoint_rule_parameters(
    0.0, -1.0,
    2.0, 0.0
);

/**
  * @param x current position of the particle
  * @param x_prev previous position of the particle
  * @param u current speed of flow field
  * @param u_prev previous speed of flow field
  * @param w proposed noise term
  * @param w_prev previous noise term
  * @param delta timestep
  * @param p struct of parameters to inform what type of milstein this is
  */
template <class T>
inline T milstein(const T x, const T x_prev, const T u, const T u_prev, const T w, const T w_prev, const T delta, const MilsteinParameters<T>& p) {
    const T term_1 = -p.alpha_1 * x - p.alpha_0 * x_prev;
    const T term_2 = delta * (p.beta_1 * u + p.beta_0 * u_prev);
    const T term_3 = p.gamma_1 * w + p.gamma_0 * w_prev;

    return term_1 + term_2 + term_3;
}

inline double adams_bashforth_milstein(const double x, const double x_prev, const double u, const double u_prev, const double w, const double w_prev, const double delta) {
    return milstein(x, x_prev, u, u_prev, w, w_prev, delta, adams_bashforth_parameters);
}

inline double midpoint_rule_milstein(const double x, const double x_prev, const double u, const double u_prev, const double w, const double w_prev, const double delta) {
    return milstein(x, x_prev, u, u_prev, w, w_prev, delta, midpoint_rule_parameters);
}

inline double euler_maruyama(const double x, const double u, const double w, const double delta) {
    return x + w + u * delta;
}
