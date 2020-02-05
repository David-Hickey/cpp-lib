#include <cstddef>
#include <stdexcept>

template <class T, size_t N, size_t M> class Tensor;

struct Index {
    size_t i, j;
};

template <class T, size_t N, size_t M>
struct ConstRow {
    const Tensor<T, N, M>* tensor;
    const size_t index;

    const T& operator[](const size_t i) const {
        return tensor->data[tensor->flatten(index, i)];
    }

    const T& at(const size_t i) const {
        if (i >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }
};

template <class T, size_t N, size_t M>
struct Row {
    Tensor<T, N, M>* tensor;
    const size_t index;

    T& operator[](const size_t i) {
        return tensor->data[tensor->flatten(index, i)];
    }

    const T& operator[](const size_t i) const {
        return tensor->data[tensor->flatten(index, i)];
    }

    T& at(const size_t i) {
        if (i >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }

    const T& at(const size_t i) const {
        if (i >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }
};


template <class T, size_t N, size_t M>
class Tensor {

    friend class Row<T, N, M>;
    friend class ConstRow<T, N, M>;

private:
    size_t flatten(size_t i, size_t j) const {
        return i * M + j;
    }

    size_t flatten(const Index& i) const {
        return this->flatten(i.i, i.j);
    }

public:
    T data[N * M];

    // 2 ways to access tensor data: with Index struct or using Row proxy
    // object.
    T& operator[](const Index& i) {
        return this->data[this->flatten(i)];
    }

    const T& operator[](const Index& i) const {
        return this->data[this->flatten(i)];
    }

    Row<T, N, M> operator[](const size_t i) {
        return Row<T, N, M>{this, i};
    }

    ConstRow<T, N, M> operator[](const size_t i) const {
        ConstRow<T, N, M> out{this, i};

        return out;
    }

    // 3 ways to access tensor data: Index struct, Row proxy, or just passing
    // two indices.
    T& at(const Index& i) {
        return this->at(i.i, i.j);
    }

    const T& at(const Index& i) const {
        return this->at(i.i, i.j);
    }

    Row<T, N, M> at(const size_t i) {
        if (i >= N) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }

    ConstRow<T, N, M> at(const size_t i) const {
        if (i >= N) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return (*this)[i];
    }

    T& at(const size_t i, const size_t j) {
        if (i >= N || j >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return this->data[this->flatten(i, j)];
    }

    const T& at(const size_t i, const size_t j) const {
        if (i >= N || j >= M) {
            throw std::range_error("Indices out of bounds for tensor");
        }

        return this->data[this->flatten(i, j)];
    }
};

template <class T, size_t N, size_t M>
Tensor<T, N, M> from_array(const T arr[N][M]) {
    Tensor<T, N, M> out{};

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            out[{i, j}] = arr[i][j];
        }
    }

    return out;
}
