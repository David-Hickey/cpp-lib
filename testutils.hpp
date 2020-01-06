#pragma once

#include <string>
#include "arrayutils.hpp"


void assert(bool b, const std::string& msg="") {
    if (!b) {
        throw std::runtime_error(msg);
    }
}

template <class T, size_t N>
void assert_all_eq(const MathArray<T, N>& a1, const MathArray<T, N>& a2, const std::string& message) {
    for (size_t i = 0; i < N; ++i) {
        assert(a1[i] == a2[i], message);
    }
}

template <class T, size_t N>
void assert_all_approx_eq(const MathArray<T, N>& a1, const MathArray<T, N>& a2, const double tol, const std::string& message) {
    for (size_t i = 0; i < N; ++i) {
        assert(std::abs(a1[i] - a2[i]) < tol, message);
    }
}
