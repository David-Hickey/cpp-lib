#include "arrayutils.hpp"
#include "mathutils.hpp"
#include "boundingbox.hpp"

#include <functional>

inline MathArray<double, 3> translating_flow_at(const MathArray<double, 3>& position,
                                                const MathArray<double, 3>& sphere_position,
                                                const MathArray<double, 3>& translation_velocity,
                                                const double sphere_radius) {

    const MathArray<double, 3> new_coords = position - sphere_position;
    const double distance = magnitude(new_coords);

    MathArray<double, 3> flow_speed{};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            flow_speed[i] -= 0.75 * sphere_radius * translation_velocity[j] * delta(i, j) / distance;
            flow_speed[i] -= 0.75 * sphere_radius * translation_velocity[j] * new_coords[i] * new_coords[j] / std::pow(distance, 3);

            flow_speed[i] -= 0.75 * std::pow(sphere_radius, 3) * translation_velocity[j] * delta(i, j) / (3.0 * std::pow(distance, 3));
            flow_speed[i] += 0.75 * std::pow(sphere_radius, 3) * translation_velocity[j] * new_coords[i] * new_coords[j] / std::pow(distance, 5);
        }
    }

    return flow_speed + translation_velocity;
}

static double strain_tensor_shear(const size_t i, const size_t j, const double shear_rate) {
    if ((i == 0 && j == 2) || (i == 2 && j == 0)) {
        return shear_rate * 0.5;
    } else {
        return 0;
    }
}

static double stress_tensor_shear(const size_t i, const size_t j, const double shear_rate) {
    if (i == 0 && j == 2) {
        return shear_rate * 0.5;
    } else if (i == 2 && j == 0) {
        return -shear_rate * 0.5;
    } else {
        return 0;
    }
}

static MathArray<double, 3> rotation_vector_shear(double shear_scaling) {
    return MathArray<double, 3>{0, 0.5 * shear_scaling, 0};
}

// Assumes shear flow of form (az, 0, 0).
inline MathArray<double, 3> shear_flow_at(const MathArray<double, 3>& position,
                                          const MathArray<double, 3>& sphere_position,
                                          const double sphere_radius,
                                          const double shear_rate) {

    const MathArray<double, 3> new_coords = position - sphere_position;
    const double distance = magnitude(new_coords);

    const auto x = new_coords;
    const auto r = distance;
    const auto a = sphere_radius;

    if (distance < sphere_radius) {
        return MathArray<double, 3>{0, 0, 0};
    }

    MathArray<double, 3> flow_speed{0, 0, 0};

    for (int i = 0; i < 3; ++i) {
        double term_1 = 0;
        double term_2 = 0;
        double term_3 = 0;

        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                const double strain_tensor_value = strain_tensor_shear(j, k, shear_rate);


                if (strain_tensor_value != 0) {
                    term_1 -= 2.5 * std::pow(a, 3) / std::pow(r, 5) * x[i] * x[j] * x[k] * strain_tensor_value;

                    const double factor = 0.5 * std::pow(a / r, 5) * strain_tensor_value;
                    term_2 -= factor * (delta(i, j) * x[k] + delta(i, k) * x[j]);
                    term_3 += factor * 5 * x[i] * x[j] * x[k] / std::pow(r, 2);
                }
            }
        }

        flow_speed[i] += term_1 + term_2 + term_3;
    }

    const MathArray<double, 3> rotation_vector = rotation_vector_shear(shear_rate);
    flow_speed -= rotation_vector.cross(new_coords) * std::pow(sphere_radius / distance, 3);

    // What's the shearing in the sphere's rest frame?
    flow_speed += translating_flow_at(position, sphere_position, MathArray<double, 3>{shear_rate * position[2]}, sphere_radius);

    return flow_speed;
}

inline MathArray<double, 3> stokes_drag(const MathArray<double, 3>& velocity, const double shear_viscosity, const double radius) {
    return (6 * M_PI * shear_viscosity * radius) * velocity;
}

inline double calculate_divergence(const std::function<MathArray<double, 3>(MathArray<double, 3>)>& f, const MathArray<double, 3>& position, const double dx=1e-10) {
    double divergence = 0;

    for (size_t i = 0; i < 3; ++i) {
        const auto position_plus = position.copy_add_index(i, dx / 2.0);
        const auto position_minus = position.copy_add_index(i, -dx / 2.0);

        divergence += (f(position_plus)[i] - f(position_minus)[i]) / dx;
    }

    return divergence;
}

static inline MathArray<double, 3> transform_position(const MathArray<double, 3>& position, const double zmin) {
    return position.copy_add_index(2, -zmin);
}

inline MathArray<double, 3> blake_tensor_at(const MathArray<double, 3>& position,
                                            const MathArray<double, 3>& real_sphere_location,
                                            const MathArray<double, 3>& force,
                                            const double z_min,
                                            const double shear_viscosity,
                                            const bool include_translation_terms=true) {

    const MathArray<double, 3> transformed_position = transform_position(position, z_min);
    const MathArray<double, 3> transformed_sphere_position = transform_position(real_sphere_location, z_min);

    const MathArray<double, 3> r = transformed_position - transformed_sphere_position;
    const MathArray<double, 3> image_sphere_location{transformed_sphere_position[0], transformed_sphere_position[1], -transformed_sphere_position[2]};
    const MathArray<double, 3> R = transformed_position - image_sphere_location;
    const double r_mag = magnitude(r);
    const double R_mag = magnitude(R);

    const double separation = real_sphere_location[2];

    MathArray<double, 3> flow_speed{};

    const MathArray<double, 3> force_prefactor = force / (8 * M_PI * shear_viscosity);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            const double prefactor = force_prefactor[j];


            const double term_1a = include_translation_terms ? delta(i, j) / r_mag : 0;
            const double term_1b = include_translation_terms ? r[i] * r[j] / (r_mag * r_mag * r_mag) : 0;

            const double term_2a = -delta(i, j) / R_mag;
            const double term_2b = -R[i] * R[j] / (R_mag * R_mag * R_mag);

            flow_speed[i] += prefactor * (term_1a + term_1b + term_2a + term_2b);
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            const double prefactor = force_prefactor[j];
            const int delta_term = (j == 2 ? -1 : 1);

            double derivative_term = 0;

            derivative_term += delta(i, j) * separation / std::pow(R_mag, 3);
            derivative_term -= 3 * R[i] * R[j] * separation / std::pow(R_mag, 5);

            derivative_term += R[j] * delta(i, 2) / std::pow(R_mag, 3);
            derivative_term -= R[i] * delta(j, 2) / std::pow(R_mag, 3);

            derivative_term -= delta(i, j) * R[2] / std::pow(R_mag, 3);
            derivative_term += 3 * R[i] * R[2] * R[j] / std::pow(R_mag, 5);

            flow_speed[i] += 2 * delta_term * separation * prefactor * derivative_term;
        }
    }

    return flow_speed;
}

inline MathArray<double, 3> blake_tensor_at(const MathArray<double, 3>& position,
                                            const MathArray<double, 3>& real_sphere_location,
                                            const MathArray<double, 3>& force,
                                            const BoundingBox& bb,
                                            const double shear_viscosity) {

    return blake_tensor_at(position, real_sphere_location, force, bb.get_zmin(), shear_viscosity);
}
