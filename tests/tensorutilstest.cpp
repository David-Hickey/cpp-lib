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
    int arr[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    Tensor<int, 2, 3> t = from_array<int, 2, 3>(arr);

    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 3; ++j) {
            assert(t[{i, j}] == arr[i][j], "Failed from_array test");
            assert(t[i][j] == arr[i][j], "Failed from_array test");
        }
    }
}

int main() {
    test_access();
    test_from_array();
}
