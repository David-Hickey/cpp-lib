#include "tensorutils.hpp"
#include "testutils.hpp"

void test_access() {
    Tensor<int, 2, 2> t{0, 1,
                        2, 3};

    t[{1, 1}] = 409;

    assert(t[{1, 1}] == 409);
    assert(t[{0, 0}] == 0);
}

int main() {
    test_access();
}
