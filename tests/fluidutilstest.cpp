#include <functional>
#include <iostream>
#include <sstream>

#include "fluidutils.hpp"
#include "testutils.hpp"

void test_stokes_drag() {
    const MathArray<double, 3> velocity{-1, 0, 0};
    const MathArray<double, 3> velocity_2{3, 2, 1};

    const MathArray<double, 3> drag_1 = stokes_drag(velocity, 1 / M_PI, 1);
    const MathArray<double, 3> analytical_drag_1{-6, 0, 0};

    const MathArray<double, 3> drag_2 = stokes_drag(velocity_2, 1 / M_PI, 1);
    const MathArray<double, 3> analytical_drag_2{18, 12, 6};

    assert(distance_between(drag_1, analytical_drag_1) < 1e-10, "Failed stokes drag calculation");
    assert(distance_between(drag_2, analytical_drag_2) < 1e-10, "Failed stokes drag calculation");
}


std::function<MathArray<double, 3>(const MathArray<double, 3>&, const MathArray<double, 3>&)> curry_blake_tester(const BoundingBox& bb, const MathArray<double, 3> real_sphere_location, const double shear_viscosity) {
    return [=](const MathArray<double, 3>& position, const MathArray<double, 3>& force) {
        return blake_tensor_at(position, real_sphere_location, force, bb, shear_viscosity);
    };
}

void test_blake() {
    const BoundingBox bb(-100, +100, -100, +100, 0, 200);
    const MathArray<double, 3> real_sphere_location{0, 0, 10};
    const double shear_viscosity = 1;

    // Curried function to test blake tensor.
    auto blake_tester = curry_blake_tester(bb, real_sphere_location, shear_viscosity);

    // Test boundary condition
    assert(all(abs(blake_tester(MathArray<double, 3>{-10, 0, 0}, MathArray<double, 3>{-1, 0, 0}) - MathArray<double, 3>{0, 0, 0}) < 1e-12), "Failed blake tensor 1");
    assert(all(abs(blake_tester(MathArray<double, 3>{3, -8, 0}, MathArray<double, 3>{-1, 0, 0}) - MathArray<double, 3>{0, 0, 0}) < 1e-12), "Failed blake tensor 2");
    assert(all(abs(blake_tester(MathArray<double, 3>{3, -8, 0}, MathArray<double, 3>{-1, 5, 0}) - MathArray<double, 3>{0, 0, 0}) < 1e-12), "Failed blake tensor 3");

    // Test agreement with Andrej, since we're pretty sure he's right. :)
    const MathArray<double, 3> point_force = MathArray<double, 3>{1, 5, 2} * (8 * M_PI);
    const MathArray<double, 3> position{3, -8, 17};
    const MathArray<double, 3> andrej_answer = MathArray<double, 3>{-0.0231991, 0.370484, -0.169464};
    const MathArray<double, 3> my_answer = blake_tester(position, point_force);
    const MathArray<double, 3> diff = abs(andrej_answer - my_answer);
    const MathArray<double, 3> ratio = diff / andrej_answer;
    const MathArray<double, 3> wrongness = abs(ratio);

    // Basically assume they differ by under 1 in part in 1e-5. A number chosen
    // because that's about the precision of what I typed for Andrej's answer.
    assert(all(wrongness < 1e-5), "Failed comparison with Andrej");
}

void test_translation() {
    const double z = 10;
    const MathArray<double, 3> sphere_position{0, 0, z};
    double radius = 1;

    // Test 1: if sphere radius is 0 then should be no change.
    {
        const MathArray<double, 3> unperturbed_velocity{1, 4, -13};
        const MathArray<double, 3> trans = translating_flow_at(MathArray<double, 3>{1, 1, 1}, sphere_position, unperturbed_velocity, 0);
        assert_all_eq(trans, unperturbed_velocity, "Failed radius 0 check");
    }

    // Test 2: should be symmetric around flow axis.
    {
        const double x_offset = -10;
        const MathArray<double, 3> unperturbed_velocity{1, 0, 0};

        // Offset to symmetry axis, diametrically opposite
        const MathArray<double, 3> trans_above = translating_flow_at(MathArray<double, 3>{x_offset, 0, z+5}, sphere_position, unperturbed_velocity, radius);
        const MathArray<double, 3> trans_below = translating_flow_at(MathArray<double, 3>{x_offset, 0, z-5}, sphere_position, unperturbed_velocity, radius);

        // Along symmetry axis
        const MathArray<double, 3> trans_middle = translating_flow_at(MathArray<double, 3>{x_offset, 0, z}, sphere_position, unperturbed_velocity, radius);

        assert(trans_above[0] == trans_below[0] && trans_above[1] == -trans_below[1] && trans_above[2] == -trans_below[2], "Failed symmetry condition 1a");
        assert(trans_middle[1] == 0 && trans_middle[2] == 0, "Failed symmetry condition 1c");
    }

    // Test 3: should be symmetric around flow axes regardless of which axis it is and how they are diametrically opposite.
    {
        const double y_offset = -10;
        const MathArray<double, 3> unperturbed_velocity{0, 1, 0};

        // Offset to symmetry axis, diametrically opposite
        const MathArray<double, 3> trans_above = translating_flow_at(MathArray<double, 3>{5, y_offset, z+5}, sphere_position, unperturbed_velocity, radius);
        const MathArray<double, 3> trans_below = translating_flow_at(MathArray<double, 3>{-5, y_offset, z-5}, sphere_position, unperturbed_velocity, radius);

        // Along symmetry axis
        const MathArray<double, 3> trans_middle = translating_flow_at(MathArray<double, 3>{0, y_offset, z}, sphere_position, unperturbed_velocity, radius);

        assert(trans_above[0] == -trans_below[0] && trans_above[1] == trans_below[1] && trans_above[2] == -trans_below[2], "Failed symmetry condition 2a");
        assert(trans_middle[0] == 0 && trans_middle[2] == 0, "Failed symmetry condition 2c");
    }

    // Test 4: non-slip boundary
    {
        const MathArray<double, 3> unperturbed_velocity{0, 1, 0};

        for (size_t index = 0; index < 3; ++index) {
            for (int plusminus : MathArray<int, 2>{+1, -1}) {
                MathArray<double, 3> surface_point(sphere_position);
                surface_point[index] += plusminus * radius;

                const auto flow = translating_flow_at(surface_point, sphere_position, unperturbed_velocity, radius);

                assert(all(abs(flow) < 1e-12), "Failed non-slip surface condition");
            }
        }
    }
}

void test_shear() {
    const MathArray<double, 3> sphere_position{0, 0, 10};
    const double shear_rate = 1;
    const double radius = 1;

    // Test 1: zero radius
    {
        const MathArray<double, 3> pos{1, 2, 3};
        const MathArray<double, 3> shear_flow = shear_flow_at(pos, sphere_position, 0, shear_rate);
        assert_all_eq(shear_flow, MathArray<double, 3>{shear_rate * pos[2], 0, 0}, "Failed shear test 1");
    }

    // // Test 2: non-slip boundary
    // {
    //     const MathArray<double, 3> unperturbed_velocity{0, 1, 0};
    //
    //     for (size_t index = 0; index < 3; ++index) {
    //         for (int plusminus : MathArray<int, 2>{+1, -1}) {
    //             MathArray<double, 3> surface_point(sphere_position);
    //             surface_point[index] += plusminus * radius;
    //
    //             std::cout << std::endl << "=== Testing at " << (plusminus > 0 ? "+" : "-") << index << " ===" << std::endl;
    //             const auto flow = shear_flow_at(surface_point, sphere_position, radius, shear_rate);
    //
    //             std::stringstream ss;
    //             ss << "Failed non-slip surface condition for ";
    //             ss << (plusminus > 0 ? "+" : "-");
    //             ss << index;
    //             const std::string err_msg = ss.str();
    //
    //
    //             if(!(all(abs(flow) < 1e-12))){
    //                 std::cout << err_msg << std::endl;
    //             }
    //         }
    //     }
    // }
}

int main() {
    test_stokes_drag();
    test_blake();
    test_translation();
    test_shear();
}
