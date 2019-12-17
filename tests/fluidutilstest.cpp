#include <functional>
#include <iostream>

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
    const double shear_viscosity = M_PI;

    // Curried function to test blake tensor.
    auto blake_tester = curry_blake_tester(bb, real_sphere_location, shear_viscosity);

    // Test with a nice x-parallel stokes drag
    assert(all(abs(blake_tester(MathArray<double, 3>{-10, 0, 0}, MathArray<double, 3>{-1, 0, 0}) - MathArray<double, 3>{0, 0, 0}) < 1e-12), "Failed blake tensor 1");
    assert(all(abs(blake_tester(MathArray<double, 3>{3, -8, 0}, MathArray<double, 3>{-1, 0, 0}) - MathArray<double, 3>{0, 0, 0}) < 1e-12), "Failed blake tensor 2");
    assert(all(abs(blake_tester(MathArray<double, 3>{3, -8, 0}, MathArray<double, 3>{-1, 5, 0}) - MathArray<double, 3>{0, 0, 0}) < 1e-12), "Failed blake tensor 3");
    // assert(blake_tester(MathArray<double, 3>{+3, -8, 0}, MathArray<double, 3>{-1, 0, 0}) - MathArray<double, 3>{0, 0, 0}, "Failed blake tensor 2", 1e-7);
    // assert(blake_tester(MathArray<double, 3>{+3, -8, 0}, MathArray<double, 3>{-1, +5, -6}) - MathArray<double, 3>{0, 0, 0}, "Failed blake tensor 3", 1e-7);
}

int main() {
    test_stokes_drag();
    test_blake();
}
