#include "tensorutils.hpp"
#include "testutils.hpp"

void test_access() {
    {
        Tensor<int, 2, 2> t{0, 1,
                        2, 3};

        t[{1, 1}] = 409;
        t[0][1] = 4009;
        t.at(1, 0) = 49;
        t.at({0, 0}) = -1;

        // Test every combination of accessors. First one is preferred format.
        assert(t[{0, 0}] == -1, "Failed access test");
        assert(t[{0, 1}] == 4009, "Failed access test");
        assert(t[{1, 0}] == 49, "Failed access test");
        assert(t[{1, 1}] == 409, "Failed access test");

        assert(t[0][0] == -1, "Failed access test");
        assert(t[0][1] == 4009, "Failed access test");
        assert(t[1][0] == 49, "Failed access test");
        assert(t[1][1] == 409, "Failed access test");

        assert(t.at({0, 0}) == -1, "Failed access test");
        assert(t.at({0, 1}) == 4009, "Failed access test");
        assert(t.at({1, 0}) == 49, "Failed access test");
        assert(t.at({1, 1}) == 409, "Failed access test");

        assert(t.at(0, 0) == -1, "Failed access test");
        assert(t.at(0, 1) == 4009, "Failed access test");
        assert(t.at(1, 0) == 49, "Failed access test");
        assert(t.at(1, 1) == 409, "Failed access test");

        assert(t.at({0, 0}) == -1, "Failed access test");
        assert(t.at({0, 1}) == 4009, "Failed access test");
        assert(t.at({1, 0}) == 49, "Failed access test");
        assert(t.at({1, 1}) == 409, "Failed access test");

        assert(t.at(0).at(0) == -1, "Failed access test");
        assert(t.at(0).at(1) == 4009, "Failed access test");
        assert(t.at(1).at(0) == 49, "Failed access test");
        assert(t.at(1).at(1) == 409, "Failed access test");

        assert(t[0].at(0) == -1, "Failed access test");
        assert(t[0].at(1) == 4009, "Failed access test");
        assert(t[1].at(0) == 49, "Failed access test");
        assert(t[1].at(1) == 409, "Failed access test");

        assert(t.at(0)[0] == -1, "Failed access test");
        assert(t.at(0)[1] == 4009, "Failed access test");
        assert(t.at(1)[0] == 49, "Failed access test");
        assert(t.at(1)[1] == 409, "Failed access test");
    }

    {
        const Tensor<int, 2, 2> t{-1, 4009,
                                  49, 409};

        // Test every combination of accessors. First one is preferred format.
        assert(t[{0, 0}] == -1, "Failed const access test");
        assert(t[{0, 1}] == 4009, "Failed const access test");
        assert(t[{1, 0}] == 49, "Failed const access test");
        assert(t[{1, 1}] == 409, "Failed const access test");

        assert(t[0][0] == -1, "Failed const access test");
        assert(t[0][1] == 4009, "Failed const access test");
        assert(t[1][0] == 49, "Failed const access test");
        assert(t[1][1] == 409, "Failed const access test");

        assert(t.at({0, 0}) == -1, "Failed const access test");
        assert(t.at({0, 1}) == 4009, "Failed const access test");
        assert(t.at({1, 0}) == 49, "Failed const access test");
        assert(t.at({1, 1}) == 409, "Failed const access test");

        assert(t.at(0, 0) == -1, "Failed const access test");
        assert(t.at(0, 1) == 4009, "Failed const access test");
        assert(t.at(1, 0) == 49, "Failed const access test");
        assert(t.at(1, 1) == 409, "Failed const access test");

        assert(t.at({0, 0}) == -1, "Failed const access test");
        assert(t.at({0, 1}) == 4009, "Failed const access test");
        assert(t.at({1, 0}) == 49, "Failed const access test");
        assert(t.at({1, 1}) == 409, "Failed const access test");

        assert(t.at(0).at(0) == -1, "Failed const access test");
        assert(t.at(0).at(1) == 4009, "Failed const access test");
        assert(t.at(1).at(0) == 49, "Failed const access test");
        assert(t.at(1).at(1) == 409, "Failed const access test");

        assert(t[0].at(0) == -1, "Failed const access test");
        assert(t[0].at(1) == 4009, "Failed const access test");
        assert(t[1].at(0) == 49, "Failed const access test");
        assert(t[1].at(1) == 409, "Failed const access test");

        assert(t.at(0)[0] == -1, "Failed const access test");
        assert(t.at(0)[1] == 4009, "Failed const access test");
        assert(t.at(1)[0] == 49, "Failed const access test");
        assert(t.at(1)[1] == 409, "Failed const access test");
    }
}

void test_from_array() {
    const int arr[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    const Tensor<int, 2, 3> t = tensor_from_array<int, 2, 3>(arr);

    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 3; ++j) {
            assert(t[{i, j}] == arr[i][j], "Failed from_array test");
            assert(t[i][j] == arr[i][j], "Failed from_array test");
        }
    }

    const int flatarr[6] = {
        1, 2, 3,
        4, 5, 6
    };

    const Tensor<int, 2, 3> t2 = tensor_from_array<int, 2, 3>(flatarr);

    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 3; ++j) {
            assert(t[{i, j}] == flatarr[i * 3 + j], "Failed from_array test");
        }
    }
}

void test_to_array() {
    const Tensor<int, 2, 3> t{
        1, 2, 3,
        4, 5, 6
    };

    int a1[6];
    t.to_array(a1);

    int a2[2][3];
    t.to_array(a2);

    for (size_t i = 0; i < 6; ++i) {
        assert(a1[i] == t.data[i], "Failed flat copy");
    }

    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            assert(a2[i][j] == t[{i, j}], "Failed 2D copy");
        }
    }
}

void test_addition() {
    const Tensor<int, 2, 3> t1{1, 2, 19,
                               3, 4, 403};

    const Tensor<int, 2, 3> t2{-1, 13, 4,
                               24, 10, 0};

    const Tensor<int, 2, 3> sum = t1 + t2;

    for (size_t i = 0; i < 6; ++i) {
        assert(sum.data[i] == t1.data[i] + t2.data[i], "Failed tensor addition");
    }
}

void test_multiplication() {
    const Tensor<int, 3, 2> t1{1,  2,
                               19, 3,
                               4,  403};

    const Tensor<int, 2, 4> t2{-1, 13, 4, -6,
                               24, 10, 0, 11};

    const Tensor<int, 3, 4> prod = t1 * t2;

    const int true_result[3][4] = {
        {47, 33, 4, 16},
        {53, 277, 76, -81},
        {9668, 4082, 16, 4409}
    };

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            assert(prod[{i, j}] == true_result[i][j], "Failed multiplication");
        }
    }
}

void test_astype() {
    const Tensor<float, 3, 2> t1{1.5,  2,
                                 19, 3.9,
                                 4,  403};

    const Tensor<int, 3, 2> t2 = t1.astype<int>();

    for (size_t i = 0; i < 6; ++i) {
        assert(int(t1.data[i]) == t2.data[i], "Failed astype cast");
    }
}

int main() {
    test_access();
    test_from_array();
    test_to_array();
    test_addition();
    test_multiplication();
}
