#include "tensorutils.hpp"
#include "testutils.hpp"

void test_access() {
    Tensor<int, 2, 2> t{0, 1,
                        2, 3};

    t[{1, 1}] = 409;

    assert(t[{1, 1}] == 409);
    assert(t[{0, 0}] == 0);
}

void test_from_array() {
    int arr[2][2] = {
        {1, 2},
        {3, 4}
    };

    Tensor<int, 2, 2> t = from_array<int, 2, 2>(arr);

    for (size_t i = 0; i < 2; ++i){
        for (size_t j = 0; j < 2; ++j) {
            assert(t[{i, j}] == arr[i][j], "Failed from_array test");
        }
    }
}

int main() {
    test_access();
    test_from_array();
}
