#include "arrayutils.hpp"
#include "mathutils.hpp"
#include "boundingbox.hpp"

inline MathArray<double, 3> translating_flow_at(const MathArray<double, 3>& position, const MathArray<double, 3>& sphere_position, const MathArray<double, 3>& translation_velocity, const double sphere_radius) {
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

inline MathArray<double, 3> stokes_drag(const MathArray<double, 3>& velocity, const double shear_viscosity, const double radius) {
    return (6 * M_PI * shear_viscosity * radius) * velocity;
}

static inline MathArray<double, 3> transform_position(const MathArray<double, 3>& position, const double zmin) {
    return MathArray<double, 3>{
        position[0],
        position[1],
        position[2] - zmin
    };
}

inline MathArray<double, 3> blake_tensor_at(const MathArray<double, 3>& position, const MathArray<double, 3>& real_sphere_location, const MathArray<double, 3>& force, const double z_min, const double shear_viscosity, const bool include_translation_terms=true) {
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

    if (include_translation_terms) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                const double prefactor = force_prefactor[j];

                const double term_1a = delta(i, j) / r_mag;
                const double term_1b = r[i] * r[j] / (r_mag * r_mag * r_mag);

                const double term_2a = -delta(i, j) / R_mag;
                const double term_2b = -R[i] * R[j] / (R_mag * R_mag * R_mag);

                flow_speed[i] += prefactor * (term_1a + term_1b + term_2a + term_2b);
            }
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

inline MathArray<double, 3> blake_tensor_at(const MathArray<double, 3>& position, const MathArray<double, 3>& real_sphere_location, const MathArray<double, 3>& force, const BoundingBox& bb, const double shear_viscosity) {
    return blake_tensor_at(position, real_sphere_location, force, bb.get_zmin(), shear_viscosity);
}
