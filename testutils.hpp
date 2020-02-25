#pragma once

#include <string>
#include "arrayutils.hpp"


struct assertion_exception : public std::runtime_error {

	assertion_exception(const char* message) : std::runtime_error(message) {}
    assertion_exception(const std::string& message) : std::runtime_error(message) {}

};


void assert(bool b, const std::string& msg="") {
    if (!b) {
        throw assertion_exception(msg);
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

template <class T>
void assert_all_eq(const std::vector<T>& a1, const std::vector<T>& a2, const std::string& message) {
	if (a1.size() != a2.size()) {
		throw assertion_exception(message);
	}

    for (size_t i = 0; i < a1.size(); ++i) {
        assert(a1[i] == a2[i], message);
    }
}

template <class T>
void assert_all_approx_eq(const std::vector<T>& a1, const std::vector<T>& a2, const double tol, const std::string& message) {
	if (a1.size() != a2.size()) {
		throw assertion_exception(message);
	}

    for (size_t i = 0; i < a1.size(); ++i) {
        assert(std::abs(a1[i] - a2[i]) < tol, message);
    }
}
