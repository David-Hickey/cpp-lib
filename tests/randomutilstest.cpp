#include "testutils.hpp"
#include "randomutils.hpp"
#include <vector>

using namespace dav;

void test_cumsum() {
    const std::vector<int> v =        {1, 2, 3,  5,  7, 11, 13};
    const std::vector<int> v_cumsum = {1, 3, 6, 11, 18, 29, 42};

    assert_all_eq(cumsum(v), v_cumsum, "Failed cumsum");
}

int main() {
    test_cumsum();
}
