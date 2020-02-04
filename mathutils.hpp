#pragma once

#include <array>
#include <iostream>


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
