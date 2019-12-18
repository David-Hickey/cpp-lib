#include "boundingbox.hpp"
#include "arrayutils.hpp"
#include "testutils.hpp"

const BoundingBox bb(96, 100, 98);
const MathArray<int, 3> inside{1, 1, 1};
const MathArray<int, 3> outside_x_greater{53, 0, 0};
const MathArray<int, 3> outside_x_lesser{-57, 0, 0};
const MathArray<int, 3> outside_y_greater{0, 51, 0};
const MathArray<int, 3> outside_y_lesser{0, -51, 0};
const MathArray<int, 3> outside_z_greater{0, 0, 51};
const MathArray<int, 3> outside_z_lesser{0, 0, -52};
const MathArray<int, 3> outside{3, -51, 51};

void test_contains() {
    assert(bb.in_bounds(inside), "failed contains inside");
    assert(!bb.in_bounds(outside_x_greater), "failed contains outside 1");
    assert(!bb.in_bounds(outside_x_lesser), "failed contains outside 2");
    assert(!bb.in_bounds(outside_y_greater), "failed contains outside 3");
    assert(!bb.in_bounds(outside_y_lesser), "failed contains outside 4");
    assert(!bb.in_bounds(outside_z_greater), "failed contains outside 5");
    assert(!bb.in_bounds(outside_z_lesser), "failed contains outside 6");
    assert(!bb.in_bounds(outside), "failed contains outside 7");
}

void test_reflect() {
    assert_all_eq(bb.reflect(inside), inside, "Failed reflection inside");
    assert_all_eq(bb.reflect(outside_x_greater), MathArray<int, 3>{43, 0, 0}, "Failed reflection outside 1");
    assert_all_eq(bb.reflect(outside_x_lesser), MathArray<int, 3>{-39, 0, 0}, "Failed reflection outside 2");
    assert_all_eq(bb.reflect(outside_y_greater), MathArray<int, 3>{0, 49, 0}, "Failed reflection outside 3");
    assert_all_eq(bb.reflect(outside_y_lesser), MathArray<int, 3>{0, -49, 0}, "Failed reflection outside 4");
    assert_all_eq(bb.reflect(outside_z_greater), MathArray<int, 3>{0, 0, 47}, "Failed reflection outside 5");
    assert_all_eq(bb.reflect(outside_z_lesser), MathArray<int, 3>{0, 0, -46}, "Failed reflection outside 6");
    assert_all_eq(bb.reflect(outside), MathArray<int, 3>{3, -49, 47}, "Failed reflection outside 7");

    MathArray<int, 3> copy_inside(inside);
    MathArray<int, 3> copy_outside_x_greater(outside_x_greater);
    MathArray<int, 3> copy_outside_x_lesser(outside_x_lesser);
    MathArray<int, 3> copy_outside_y_greater(outside_y_greater);
    MathArray<int, 3> copy_outside_y_lesser(outside_y_lesser);
    MathArray<int, 3> copy_outside_z_greater(outside_z_greater);
    MathArray<int, 3> copy_outside_z_lesser(outside_z_lesser);
    MathArray<int, 3> copy_outside(outside);

    assert_all_eq(bb.reflect_situ(copy_inside), inside, "Failed reflection inside situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside_x_greater), MathArray<int, 3>{43, 0, 0}, "Failed reflection outside 1 situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside_x_lesser), MathArray<int, 3>{-39, 0, 0}, "Failed reflection outside 2 situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside_y_greater), MathArray<int, 3>{0, 49, 0}, "Failed reflection outside 3 situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside_y_lesser), MathArray<int, 3>{0, -49, 0}, "Failed reflection outside 4 situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside_z_greater), MathArray<int, 3>{0, 0, 47}, "Failed reflection outside 5 situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside_z_lesser), MathArray<int, 3>{0, 0, -46}, "Failed reflection outside 6 situ 1");
    assert_all_eq(bb.reflect_situ(copy_outside), MathArray<int, 3>{3, -49, 47}, "Failed reflection outside 7 situ 1");

    assert_all_eq(copy_inside, inside, "Failed reflection inside situ 2");
    assert_all_eq(copy_outside_x_greater, MathArray<int, 3>{43, 0, 0}, "Failed reflection outside 1 situ 2");
    assert_all_eq(copy_outside_x_lesser, MathArray<int, 3>{-39, 0, 0}, "Failed reflection outside 2 situ 2");
    assert_all_eq(copy_outside_y_greater, MathArray<int, 3>{0, 49, 0}, "Failed reflection outside 3 situ 2");
    assert_all_eq(copy_outside_y_lesser, MathArray<int, 3>{0, -49, 0}, "Failed reflection outside 4 situ 2");
    assert_all_eq(copy_outside_z_greater, MathArray<int, 3>{0, 0, 47}, "Failed reflection outside 5 situ 2");
    assert_all_eq(copy_outside_z_lesser, MathArray<int, 3>{0, 0, -46}, "Failed reflection outside 6 situ 2");
    assert_all_eq(copy_outside, MathArray<int, 3>{3, -49, 47}, "Failed reflection outside 7 situ 2");

    assert(bb.reflect_z(inside[2]) == inside[2], "Failed scalar reflection z");
    assert(bb.reflect_z(outside[2]) == 47, "Failed scalar reflection z");
    assert(bb.reflect_y(inside[1]) == inside[1], "Failed scalar reflection y");
    assert(bb.reflect_y(outside[1]) == -49, "Failed scalar reflection y");
    assert(bb.reflect_x(inside[0]) == inside[0], "Failed scalar reflection x");
    assert(bb.reflect_x(outside_x_greater[0]) == 43, "Failed scalar reflection x");
}

void test_volume() {
    assert(bb.volume() == 100*98*96, "Failed volume calculation");
}

void test_getters() {
    assert(bb.get_xmin() == -48, "Failed get_xmin");
    assert(bb.get_xmax() == 48, "Failed get_xmax");
    assert(bb.get_ymin() == -50, "Failed get_ymin");
    assert(bb.get_ymax() == 50, "Failed get_ymax");
    assert(bb.get_zmin() == -49, "Failed get_zmin");
    assert(bb.get_zmax() == 49, "Failed get_zmax");

    assert(bb.get_xsize() == 96, "Failed get_xsize");
    assert(bb.get_ysize() == 100, "Failed get_ysize");
    assert(bb.get_zsize() == 98, "Failed get_zsize");
}

void test_constructors() {
    const BoundingBox bb1(-5, +5, -10, +10, -15, +15);
    const BoundingBox bb2(10, 20, 30);
    const BoundingBox bb3(bb1);

    assert_all_eq(bb1.get_lower_bounds(), bb2.get_lower_bounds(), "3-argument constructor failed");
    assert_all_eq(bb1.get_upper_bounds(), bb2.get_upper_bounds(), "3-argument constructor failed");

    assert_all_eq(bb1.get_lower_bounds(), bb3.get_lower_bounds(), "copy constructor failed");
    assert_all_eq(bb1.get_upper_bounds(), bb3.get_upper_bounds(), "copy constructor failed");
}

int main() {
    test_contains();
    test_reflect();
    test_volume();
    test_getters();
    test_constructors();
}
