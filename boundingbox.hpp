#pragma once

#include <random>
#include "arrayutils.hpp"


class BoundingBox {

public:
    BoundingBox(const double x_min, const double x_max, const double y_min, const double y_max, const double z_min, const double z_max)
    : x_min(x_min), x_max(x_max)
    , y_min(y_min), y_max(y_max)
    , z_min(z_min), z_max(z_max)
    , lower_bounds{x_min, y_min, z_min}
    , upper_bounds{x_max, y_max, z_max} {}

    BoundingBox(const double x_dim, const double y_dim, const double z_dim)
    : x_min(-x_dim / 2.0), x_max(x_dim / 2.0)
    , y_min(-y_dim / 2.0), y_max(y_dim / 2.0)
    , z_min(-z_dim / 2.0), z_max(z_dim / 2.0)
    , lower_bounds{x_min, y_min, z_min}
    , upper_bounds{x_max, y_max, z_max} {}

    template <class T>
    inline bool in_bounds(const T x, const T y, const T z) const {
        return (x > this->x_min && x < this->x_max)
            && (y > this->y_min && y < this->y_max)
            && (z > this->z_min && z < this->z_max);
    };

    template <class T>
    inline bool in_bounds(const MathArray<T, 3>& point) const {
        return this->in_bounds(point[0], point[1], point[2]);
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
        return MathArray<double, 3>{
            std::uniform_real_distribution<double>(this->x_min, this->x_max)(engine),
            std::uniform_real_distribution<double>(this->y_min, this->y_max)(engine),
            std::uniform_real_distribution<double>(this->z_min, this->z_max)(engine)
        };
    }

    inline double volume() const {
        return (this->x_max - this->x_min)
             * (this->y_max - this->y_min)
             * (this->z_max - this->z_min);
    }

    inline double get_xmin() const { return this->x_min; };
    inline double get_xmax() const { return this->x_max; };
    inline double get_ymin() const { return this->y_min; };
    inline double get_ymax() const { return this->y_max; };
    inline double get_zmin() const { return this->z_min; };
    inline double get_zmax() const { return this->z_max; };
    inline double get_xsize() const { return this->x_max - this->x_min; };
    inline double get_ysize() const { return this->y_max - this->y_min; };
    inline double get_zsize() const { return this->z_max - this->z_min; };

private:
    const double x_min, x_max;
    const double y_min, y_max;
    const double z_min, z_max;
    const MathArray<double, 3> lower_bounds;
    const MathArray<double, 3> upper_bounds;

    template <class T>
    T reflect_through(T coordinate, double bound) const {
        return 2 * bound - coordinate;
    }

    template <class T>
    T reflect_through_index(T coordinate, int index) const {
        if (coordinate > this->upper_bounds[index]) {
            return 2 * this->upper_bounds[index] - coordinate;
        } else if (coordinate < this->lower_bounds[index]) {
            return 2 * this->lower_bounds[index] - coordinate;
        } else {
            return coordinate;
        }
    }

};
