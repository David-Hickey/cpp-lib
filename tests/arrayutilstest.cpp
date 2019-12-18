#include "arrayutils.hpp"
#include "testutils.hpp"

void test_all() {
    assert(!all(MathArray<bool, 4>{false, false, false, true}));
    assert(all(MathArray<bool, 5>{true, true, true, true, true}));
    assert(!all(MathArray<bool, 2>{false, false}));
}

void test_any() {
    assert(any(MathArray<bool, 4>{false, false, false, true}));
    assert(any(MathArray<bool, 5>{true, true, true, true, true}));
    assert(!any(MathArray<bool, 2>{false, false}));
}

void test_boolean_negate() {
    const MathArray<bool, 4> a1{true, false, true, true};
    const MathArray<bool, 4> analytical_invert{false, true, false, false};

    assert_all_eq(!a1, analytical_invert, "Failed boolean inversion");
}

void test_numerical_negate() {
    const MathArray<int, 3> a1{5, -7, 0};
    const MathArray<int, 3> analytical_invert{-5, 7, 0};

    assert_all_eq(-a1, analytical_invert, "Failed negation");
}

void test_addition() {
    const MathArray<int, 5> a1{3, 5, 7, 11, 13};
    const MathArray<int, 5> a2{17, 23, 27, 31, 37};
    const MathArray<int, 5> analytical_array_sum{20, 28, 34, 42, 50};
    const int scalar = 17;
    const MathArray<int, 5> analytical_scalar_sum{20, 22, 24, 28, 30};


    // Regular addition
    {
        const MathArray<int, 5> sum = a1 + a2;
        assert_all_eq(analytical_array_sum, sum, "Failed regular array addition");
    }

    // In-place addition
    {
        MathArray<int, 5> sum(a1);
        assert_all_eq(sum += a2, sum, "Failed in-place array addition");
        assert_all_eq(analytical_array_sum, sum, "Failed in-place array addition");
    }

    // Regular scalar addition
    {
        const MathArray<int, 5> sum = a1 + scalar;
        assert_all_eq(analytical_scalar_sum, sum, "Failed regular scalar addition");
    }

    // In-place scalar addition
    {
        MathArray<int, 5> sum(a1);
        assert_all_eq(analytical_scalar_sum, sum += scalar, "Failed in-place scalar addition");
        assert_all_eq(analytical_scalar_sum, sum, "Failed in-place scalar addition");
    }

    // Reversed regular scalar addition
    {
        const MathArray<int, 5> sum = scalar + a1;
        assert_all_eq(analytical_scalar_sum, sum, "Failed reversed regular scalar addition");
    }
}

void test_subtraction() {
    const MathArray<int, 5> a1{3, 23, 7, 11, 37};
    const MathArray<int, 5> a2{17, 5, 27, 31, 37};
    const MathArray<int, 5> analytical_array_diff{-14, 18, -20, -20, 0};
    const int scalar = 17;
    const MathArray<int, 5> analytical_scalar_diff{0, -12, 10, 14, 20};

    // Regular subtraction
    {
        const MathArray<int, 5> diff = a1 - a2;
        assert_all_eq(analytical_array_diff, diff, "Failed regular array subtraction");
    }

    // In-place subtraction
    {
        MathArray<int, 5> diff(a1);
        assert_all_eq(analytical_array_diff, diff -= a2, "Failed in-place array subtraction");
        assert_all_eq(analytical_array_diff, diff, "Failed in-place array subtraction");
    }

    // Regular scalar subtraction
    {
        const MathArray<int, 5> diff = a2 - scalar;
        assert_all_eq(analytical_scalar_diff, diff, "Failed regular scalar subtraction");
    }

    // In-place scalar subtraction
    {
        MathArray<int, 5> diff(a2);
        assert_all_eq(analytical_scalar_diff, diff -= scalar, "Failed in-place scalar subtraction");
        assert_all_eq(analytical_scalar_diff, diff, "Failed in-place scalar subtraction");
    }

    // Reversed Regular scalar subtraction
    {
        const MathArray<int, 5> diff = scalar - a2;
        assert_all_eq(analytical_scalar_diff, -diff, "Failed reversed regular scalar subtraction");
    }
}

void test_multiplication() {
    const MathArray<int, 5> a1{3, 5, 7, 11, 13};
    const MathArray<int, 5> a2{17, 23, 27, 31, 37};
    const MathArray<int, 5> analytical_array_prod{51, 115, 189, 341, 481};
    const int scalar = 17;
    const MathArray<int, 5> analytical_scalar_prod{51, 85, 119, 187, 221};


    // Regular multiplication
    {
        const MathArray<int, 5> prod = a1 * a2;
        assert_all_eq(analytical_array_prod, prod, "Failed regular array multiplication");
    }

    // In-place multiplication
    {
        MathArray<int, 5> prod(a1);
        assert_all_eq(analytical_array_prod, prod *= a2, "Failed in-place array multiplication");
        assert_all_eq(analytical_array_prod, prod, "Failed in-place array multiplication");
    }

    // Regular scalar multiplication
    {
        const MathArray<int, 5> prod = a1 * scalar;
        assert_all_eq(analytical_scalar_prod, prod, "Failed regular scalar multiplication");
    }

    // In-place scalar multiplication
    {
        MathArray<int, 5> prod(a1);
        assert_all_eq(analytical_scalar_prod, prod *= scalar, "Failed in-place scalar multiplication");
        assert_all_eq(analytical_scalar_prod, prod, "Failed in-place scalar multiplication");
    }

    // Reversed regular scalar multiplication
    {
        const MathArray<int, 5> prod = scalar * a1;
        assert_all_eq(analytical_scalar_prod, prod, "Failed reversed regular scalar multiplication");
    }
}

void test_equals() {
    const MathArray<int, 5> a1{3, 5, 7, 11, 13};
    const MathArray<int, 5> a2{3, 5, 9, -11, 13};
    const int scalar = 7;
    const MathArray<bool, 5> analytical_equality{true, true, false, false, true};
    const MathArray<bool, 5> analytical_equality_scalar{false, false, true, false, false};

    assert_all_eq(analytical_equality, a1 == a2, "Failed equality eq");
    assert_all_eq(analytical_equality_scalar, a1 == scalar, "Failed equality eq scalar");
    assert_all_eq(analytical_equality_scalar, scalar == a1, "Failed equality eq scalar reversed");
}

void test_nequals() {
    const MathArray<int, 5> a1{3, 5, 7, 11, 13};
    const MathArray<int, 5> a2{3, 5, 9, -11, 13};
    const MathArray<bool, 5> analytical_equality{false, false, true, true, false};
    const int scalar = 7;
    const MathArray<bool, 5> analytical_nequality{false, false, true, true, false};
    const MathArray<bool, 5> analytical_nequality_scalar{true, true, false, true, true};

    assert_all_eq(analytical_equality, a1 != a2, "Failed nequality eq");
    assert_all_eq(analytical_nequality_scalar, a1 != scalar, "Failed nequality eq scalar");
    assert_all_eq(analytical_nequality_scalar, scalar != a1, "Failed nequality eq scalar reversed");
}

void test_division() {
    const MathArray<int, 5> a1{17, 46, 7, 1, 80};
    const MathArray<int, 5> a2{17, 23, 27, 31, 37};
    const MathArray<int, 5> analytical_array_div{1, 2, 0, 0, 2};
    const int scalar = 17;
    const MathArray<int, 5> analytical_scalar_div{1, 2, 0, 0, 4};
    const MathArray<int, 5> analytical_scalar_div_recip{1, 0, 2, 17, 0};


    // Regular division
    {
        const MathArray<int, 5> div = a1 / a2;
        assert_all_eq(analytical_array_div, div, "Failed regular array division");
    }

    // In-place division
    {
        MathArray<int, 5> div(a1);
        assert_all_eq(analytical_array_div, div /= a2, "Failed in-place array division");
        assert_all_eq(analytical_array_div, div, "Failed in-place array division");
    }

    // Regular scalar division
    {
        const MathArray<int, 5> div = a1 / scalar;
        assert_all_eq(analytical_scalar_div, div, "Failed regular scalar division");
    }

    // In-place scalar division
    {
        MathArray<int, 5> div(a1);
        assert_all_eq(analytical_scalar_div, div /= scalar, "Failed in-place scalar division");
        assert_all_eq(analytical_scalar_div, div, "Failed in-place scalar division");
    }

    // Reversed regular scalar division
    {
        const MathArray<int, 5> div = scalar / a1;
        assert_all_eq(analytical_scalar_div_recip, div, "Failed reversed regular scalar division");
    }
}

void test_inequalities_greater() {
    const MathArray<int, 5> a1{3, 23, 7, 11, 37};
    const MathArray<int, 5> a2{17, 5, 27, 31, 37};
    const MathArray<bool, 5> analytical_array_gt{true, false, true, true, false};
    const MathArray<bool, 5> analytical_array_gte{true, false, true, true, true};
    const int scalar = 11;
    const MathArray<bool, 5> analytical_scalar_gt{false, true, false, false, true};
    const MathArray<bool, 5> analytical_scalar_gte{false, true, false, true, true};
    const MathArray<bool, 5> analytical_scalar_gt_rev{true, false, true, false, false};
    const MathArray<bool, 5> analytical_scalar_gte_rev{true, false, true, true, false};

    assert_all_eq(a2 > a1, analytical_array_gt, "Failed greater than");
    assert_all_eq(a2 >= a1, analytical_array_gte, "Failed greater than equal to");

    assert_all_eq(a1 > scalar, analytical_scalar_gt, "Failed greater than scalar");
    assert_all_eq(a1 >= scalar, analytical_scalar_gte, "Failed greater than equal to scalar");

    assert_all_eq(scalar > a1, analytical_scalar_gt_rev, "Failed greater than scalar reversed");
    assert_all_eq(scalar >= a1, analytical_scalar_gte_rev, "Failed greater than equal to scalar reversed");
}

void test_inequalities_lesser() {
    const MathArray<int, 5> a1{3, 23, 7, 11, 37};
    const MathArray<int, 5> a2{17, 5, 27, 31, 37};
    const MathArray<bool, 5> analytical_array_lt{false, true, false, false, false};
    const MathArray<bool, 5> analytical_array_lte{false, true, false, false, true};
    const int scalar = 11;
    const MathArray<bool, 5> analytical_scalar_lt{true, false, true, false, false};
    const MathArray<bool, 5> analytical_scalar_lte{true, false, true, true, false};
    const MathArray<bool, 5> analytical_scalar_lt_rev{false, true, false, false, true};
    const MathArray<bool, 5> analytical_scalar_lte_rev{false, true, false, true, true};

    assert_all_eq(a2 < a1, analytical_array_lt, "Failed less than");
    assert_all_eq(a2 <= a1, analytical_array_lte, "Failed less than equal to");

    assert_all_eq(a1 < scalar, analytical_scalar_lt, "Failed less than scalar");
    assert_all_eq(a1 <= scalar, analytical_scalar_lte, "Failed less than equal to scalar");

    assert_all_eq(scalar < a1, analytical_scalar_lt_rev, "Failed less than scalar reversed");
    assert_all_eq(scalar <= a1, analytical_scalar_lte_rev, "Failed less than equal to scalar reversed");
}

void test_boolean_misc_ops() {
    const MathArray<bool, 4> a1{true, true, false, false};
    const MathArray<bool, 4> a2{true, false, false, true};

    const MathArray<bool, 4> analytical_or{true, true, false, true};
    const MathArray<bool, 4> analytical_and{true, false, false, false};

    assert_all_eq(a1 || a2, analytical_or, "failed analytical or");
    assert_all_eq(a1 && a2, analytical_and, "failed analytical and");
}

void test_magnitude_distance() {
    MathArray<int, 3> a1{10, 10, 0};
    MathArray<int, 3> a2{7, 6, 0};
    MathArray<int, 3> a3{3, 4};

    assert(magnitude_sq(a3) == 25, "Failed magnitude_sq");
    assert(magnitude(a3) == 5, "Failed magnitude_sq");
    assert(distance_between_sq(a1, a2) == 25, "Failed distance_sq");
    assert(distance_between(a1, a2) == 5, "Failed distance");
}

void test_sum() {
    MathArray<int, 5> a1{13, 7, 23, 0, 9};

    assert(a1.sum() == 52, "Failed summation");
    assert_all_eq(a1.cumsum(), MathArray<int, 5>{13, 20, 43, 43, 52}, "Failed cumsum");
}

void test_prod() {
    MathArray<int, 5> a1{13, 7, 23, 0, 9};
    MathArray<int, 5> a2{13, 7, 23, -1, 9};

    assert(a1.prod() == 0, "Failed product");
    assert(a2.prod() == -18837, "Failed product");

    assert_all_eq(a1.cumprod(), MathArray<int, 5>{13, 91, 2093, 0, 0}, "Failed cumprod 1");
    assert_all_eq(a2.cumprod(), MathArray<int, 5>{13, 91, 2093, -2093, -18837}, "Failed cumprod 2");
}

void test_set() {
    MathArray<int, 5> a1{1, 2, 3, 4, 5};

    // rvalue copy set test
    assert_all_eq(a1.copy_set(3, -19), MathArray<int, 5>{1, 2, 3, -19, 5}, "Failed copy_set rvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, 4, 5}, "Failed copy_set rvalue");

    // rvalue set test
    assert_all_eq(a1.set(3, -17), MathArray<int, 5>{1, 2, 3, -17, 5}, "Failed set rvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, -17, 5}, "Failed set rvalue");


    int set1 = 43;
    int set2 = 807;
    // lvalue copy set test
    assert_all_eq(a1.copy_set(3, set1), MathArray<int, 5>{1, 2, 3, set1, 5}, "Failed copy_set lvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, -17, 5}, "Failed copy_set lvalue");

    // lvalue set test
    assert_all_eq(a1.set(3, set2), MathArray<int, 5>{1, 2, 3, set2, 5}, "Failed set lvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, set2, 5}, "Failed set lvalue");
}

void test_add_index() {
    MathArray<int, 5> a1{1, 2, 3, 4, 5};

    // rvalue copy set test
    assert_all_eq(a1.copy_add_index(3, -19), MathArray<int, 5>{1, 2, 3, -15, 5}, "Failed copy_set rvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, 4, 5}, "Failed copy_set rvalue");

    // rvalue set test
    assert_all_eq(a1.add_index(3, -17), MathArray<int, 5>{1, 2, 3, -13, 5}, "Failed set rvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, -13, 5}, "Failed set rvalue");


    int set1 = 43;
    int set2 = 807;
    // lvalue copy set test
    assert_all_eq(a1.copy_add_index(3, set1), MathArray<int, 5>{1, 2, 3, -13+set1, 5}, "Failed copy_set lvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, -13, 5}, "Failed copy_set lvalue");

    // lvalue set test
    assert_all_eq(a1.add_index(3, set2), MathArray<int, 5>{1, 2, 3, -13+set2, 5}, "Failed set lvalue");
    assert_all_eq(a1, MathArray<int, 5>{1, 2, 3, -13+set2, 5}, "Failed set lvalue");
}

int main() {
    test_any();
    test_all();
    test_equals();
    test_nequals();

    test_inequalities_greater();
    test_inequalities_lesser();

    test_boolean_negate();
    test_numerical_negate();

    test_addition();
    test_subtraction();
    test_multiplication();
    test_division();

    test_boolean_misc_ops();

    test_magnitude_distance();

    test_sum();
    test_prod();

    test_set();
    test_add_index();

    return 0;
}
