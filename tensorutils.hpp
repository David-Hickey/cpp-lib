#include <cstddef>
#include <stdexcept>


struct Index {
    size_t i, j;
};

template <class T, size_t N, size_t M>
class Tensor {

private:
    size_t flatten(size_t i, size_t j) const {
        return i * M + j;
    }

    size_t flatten(const Index& i) const {
        return this->flatten(i.i, i.j);
    }

public:
    T data[N * M];

    T& operator[](const Index& i) {
        return this->data[this->flatten(i)];
    }

    const T& operator[](const Index& i) const {
        return this->data[this->flatten(i)];
    }

    T& at(const Index& i) {
        if (i.i >= N || i.j >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }

    const T& at(const Index& i) const {
        if (i.i >= N || i.j >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }
};