#include "mathutils.hpp"
#include "testutils.hpp"

void test_flatten() {
    const int bins_x = 13;
    const int bins_y = 11;

    int prev_flat = -1;
    for (size_t x = 0; x < bins_x; ++x) {
        for (size_t y = 0; y < bins_y; ++y) {
            const size_t flat = flatten_bin_index(x, y, bins_x, bins_y);

            assert(flat == prev_flat + 1, "Failed to increment flat index in expected order");

            const auto unflat = unflatten_bin_index(flat, bins_x, bins_y);

            assert(unflat[0] == x && unflat[1] == y, "Couldn't decode flattened index");

            prev_flat = flat;
        }
    }
}

void test_delta() {
    assert(delta(0, 1) == 0, "Failed delta");
    assert(delta(1, 1) == 1, "Failed delta");
    assert(delta(0, 0) == 1, "Failed delta");
    assert(delta(1, 0) == 0, "Failed delta");
    assert(delta(-1, 1) == 0, "Failed delta");
    assert(delta(-1, -1) == 1, "Failed delta");
}

void test_levicevita() {
    assert(levicevita(1, 2, 3) == 1, "Failed levicevita");
    assert(levicevita(2, 3, 1) == 1, "Failed levicevita");
    assert(levicevita(3, 1, 2) == 1, "Failed levicevita");
    assert(levicevita(1, 3, 2) == -1, "Failed levicevita");
    assert(levicevita(2, 1, 3) == -1, "Failed levicevita");
    assert(levicevita(3, 2, 1) == -1, "Failed levicevita");

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                if (i == j || j == k || i == k) {
                    assert(levicevita(i, j, k) == 0, "Failed levicevita");
                } else {
                    assert(levicevita(i, j, k) == 1 || levicevita(i, j, k) == -1, "Failed levicevita");
                }
            }
        }
    }
}


int main() {
    test_delta();
    test_levicevita();
    test_flatten();
}
