#pragma once

#include <array>
#include <cmath>
#include <vector>
#include <random>
#include "arrayutils.hpp"

    namespace dav {
    template <class T>
    inline const T& choice(const std::vector<T>& v, std::mt19937& engine) noexcept {
        return v[std::uniform_int_distribution<int>(0, v.size() - 1)(engine)];
    }


    template <class T, size_t N>
    inline const T& choice(const MathArray<T, N>& v, std::mt19937& engine) noexcept {
        return v[std::uniform_int_distribution<int>(0, v.size() - 1)(engine)];
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
    inline size_t find_first_element_greater_than(const std::vector<T>& v, const T& search) noexcept {
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i] > search) {
                return i;
            }
        }

        return v.size();
    }

    // template <class T>
    // inline size_t find_first_element_greater_than_sorted(const std::vector<T>& v, const T& search) noexcept {
    //     return std::distance(v.begin(), std::lower_bound(v.begin(), v.end(), search));
    // }

    /**
      * Alas, linear time rather than clever logarithmic time. Had to implement this
      * because very occasionally, std::lower_bound would return v.end() instead of
      * the correct value. I have no idea why - the vector was sorted and all... :(
      */
    template <class T, size_t N>
    inline size_t find_first_element_greater_than(const MathArray<T, N>& v, const T& search) noexcept {
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
        const T rand = std::uniform_real_distribution<T>(0, cdf.back())(engine);
        const size_t index = find_first_element_greater_than(cdf, rand);

        return index;
    }

    /**
      * Pick an index using the given weights. e.g. if weights is [1, 2, 0] then we
      * except to see '2' 0% of the time, '1' 67% of the time, and '0' 33% of the
      * time.
      */
    template <class T, size_t N>
    inline size_t weighted_index(const MathArray<T, N>& weights, std::mt19937& engine) noexcept {
        const MathArray<T, N> cdf = cumsum(weights);
        const T rand = std::uniform_real_distribution<T>(0, cdf.back())(engine);
        const size_t index = find_first_element_greater_than(cdf, rand);

        return index;
    }

    template <class T>
    inline size_t weighted_index_cdf(const std::vector<T>& cdf, std::mt19937& engine) noexcept {
        const T rand = std::uniform_real_distribution<T>(0, cdf.back())(engine);
        const size_t index = find_first_element_greater_than_sorted(cdf, rand);

        return index;
    }

    template <class T, size_t N>
    inline size_t weighted_index_cdf(const MathArray<T, N>& cdf, std::mt19937& engine) noexcept {
        const T rand = std::uniform_real_distribution<T>(0, cdf.back())(engine);
        const size_t index = find_first_element_greater_than(cdf, rand);

        return index;
    }
}
