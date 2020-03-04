#pragma once

#include "arrayutils.hpp"
#include "randomutils.hpp"

#include <random>

template <class T, size_t N>
inline constexpr MathArray<T, N> elementwise_min(const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = std::min(a1[i], a2[i]);
    }

    return output;
}

template <class T, size_t N>
inline constexpr MathArray<T, N> elementwise_max(const MathArray<T, N>& a1, const MathArray<T, N>& a2) noexcept {
    MathArray<T, N> output{};

    for (size_t i = 0; i < N; ++i) {
        output[i] = std::max(a1[i], a2[i]);
    }

    return output;
}


class BoundingBox {

public:
    BoundingBox(const double x_min, const double x_max, const double y_min, const double y_max, const double z_min, const double z_max)
    : lower_bounds{x_min, y_min, z_min}
    , upper_bounds{x_max, y_max, z_max} {}

    BoundingBox(const double x_dim, const double y_dim, const double z_dim)
    : lower_bounds{-x_dim / 2.0, -y_dim / 2.0, -z_dim / 2.0}
    , upper_bounds{+x_dim / 2.0, +y_dim / 2.0, +z_dim / 2.0} {}

    BoundingBox(const MathArray<double, 3>& corner_1, const MathArray<double, 3>& corner_2)
    : lower_bounds(elementwise_min(corner_1, corner_2))
    , upper_bounds(elementwise_max(corner_1, corner_2)) {}

    BoundingBox(const BoundingBox& other)
    : lower_bounds(other.get_lower_bounds())
    , upper_bounds(other.get_upper_bounds()) {}

    template <class T>
    inline bool in_bounds(const MathArray<T, 3>& position) const {
        for (size_t i = 0; i < 3; ++i) {
            if (position[i] < this->lower_bounds[i] || position[i] > this->upper_bounds[i]) {
                return false;
            }
        }

        return true;
    };

    template <class T>
    inline MathArray<T, 3> reflect(const MathArray<T, 3>& arr) const {
        MathArray<T, 3> output(arr);

        return reflect_situ(output);
    }

    template <class T>
    inline T reflect_z(T z) const {
        return this->reflect_through_index(z, 2);
    }

    template <class T>
    inline T reflect_y(T y) const {
        return this->reflect_through_index(y, 1);
    }

    template <class T>
    inline T reflect_x(T x) const {
        return this->reflect_through_index(x, 0);
    }

    template <class T>
    inline MathArray<T, 3>& reflect_situ(MathArray<T, 3>& arr) const {
        for (int i = 0; i < 3; ++i) {
            if (arr[i] < this->lower_bounds[i]) {
                arr[i] = this->reflect_through<T>(arr[i], this->lower_bounds[i]);
            } else if (arr[i] > this->upper_bounds[i]) {
                arr[i] = this->reflect_through<T>(arr[i], this->upper_bounds[i]);
            }
        }

        return arr;
    }

    inline MathArray<double, 3> random_point_in_bounds(std::mt19937& engine) const {
        MathArray<double, 3> output{};

        for (size_t i = 0; i < 3; ++i) {
            output[i] = std::uniform_real_distribution<double>(this->lower_bounds[i], this->upper_bounds[i])(engine);
        }

        return output;
    }

    inline double volume() const {
        return (this->upper_bounds - this->lower_bounds).prod();
    }

    inline const MathArray<double, 3>& get_upper_bounds() const {
        return this->upper_bounds;
    }

    inline const MathArray<double, 3>& get_lower_bounds() const {
        return this->lower_bounds;
    }

    /**
      * WEIGHTED BY SIZE OF SURFACE. In other words, a very elongated box would
      * see most of the points picked on the long faces and very few on the
      * small end faces.
      */
    inline MathArray<double, 3> random_point_on_surface(std::mt19937& engine) const {
        // Surface areas...
        const std::vector<double> surface_areas{
            this->get_xsurface(), this->get_xsurface(), // surfaces with x normal
            this->get_ysurface(), this->get_ysurface(), // surfaces with y normal
            this->get_zsurface(), this->get_zsurface()  // surfaces with z normal
        };
        const size_t surface_index = weighted_index(surface_areas, engine);
        const bool basis_index = surface_index / 2; // x=0, y=1, z=2.
        const bool upper = (surface_index % 3) == 1; // 0 if lower, 1 if upper.

        // Find a random point P in the box, then project it onto the chosen
        // surface. This means setting P[basis_index] to be on the appropriate
        // surface.
        const MathArray<double, 3> surface_point = this->random_point_in_bounds(engine)
            .copy_set(basis_index, upper ? this->upper_bounds[basis_index] : this->lower_bounds[basis_index]);

        return surface_point;
    }

    inline double get_ith_size(const size_t i) const { return this->upper_bounds[i] - this->lower_bounds[i]; };
    inline double get_xmin() const { return this->lower_bounds[0]; };
    inline double get_xmax() const { return this->upper_bounds[0]; };
    inline double get_ymin() const { return this->lower_bounds[1]; };
    inline double get_ymax() const { return this->upper_bounds[1]; };
    inline double get_zmin() const { return this->lower_bounds[2]; };
    inline double get_zmax() const { return this->upper_bounds[2]; };
    inline double get_xsize() const { return this->get_ith_size(0); };
    inline double get_ysize() const { return this->get_ith_size(1); };
    inline double get_zsize() const { return this->get_ith_size(2); };
    inline double get_xsurface() const { return this->get_ysize() * this->get_zsize(); }
    inline double get_ysurface() const { return this->get_zsize() * this->get_xsize(); }
    inline double get_zsurface() const { return this->get_xsize() * this->get_ysize(); }

private:
    const MathArray<double, 3> lower_bounds;
    const MathArray<double, 3> upper_bounds;

    template <class T>
    T reflect_through(const T& coordinate, const double bound) const {
        return 2 * bound - coordinate;
    }

    template <class T>
    T reflect_through_index(const T& coordinate, const int index) const {
        if (coordinate > this->upper_bounds[index]) {
            return 2 * this->upper_bounds[index] - coordinate;
        } else if (coordinate < this->lower_bounds[index]) {
            return 2 * this->lower_bounds[index] - coordinate;
        } else {
            return coordinate;
        }
    }

};
