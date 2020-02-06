#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <iterator>


template <class T, size_t N, size_t M> class Tensor;

struct Index {
    size_t i, j;
};

template <class T, size_t N, size_t M>
struct ConstRow {
    const Tensor<T, N, M>* tensor;
    const size_t index;

    const T& operator[](const size_t i) const noexcept {
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

    T& operator[](const size_t i) noexcept {
        return tensor->data[tensor->flatten(index, i)];
    }

    const T& operator[](const size_t i) const noexcept {
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

    static size_t flatten(size_t i, size_t j) noexcept {
        return i * M + j;
    }

    static size_t flatten(const Index& i) noexcept {
        return flatten(i.i, i.j);
    }

public:
    T data[N * M];

    // 2 ways to access tensor data: with Index struct or using Row proxy
    // object.
    T& operator[](const Index& i) noexcept {
        return this->data[this->flatten(i)];
    }

    const T& operator[](const Index& i) const noexcept {
        return this->data[this->flatten(i)];
    }

    Row<T, N, M> operator[](const size_t i) noexcept {
        return Row<T, N, M>{this, i};
    }

    ConstRow<T, N, M> operator[](const size_t i) const noexcept {
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

    /**
      * Copy this tensor to a 2D row-major array. The weird syntax of the
      * function signature prevents the array from decaying into a pointer,
      * so this won't compile if you pass the wrong array size.
      */
    void to_array(T (&arr)[N][M]) const {
        size_t index = 0;

        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                arr[i][j] = this->data[index++];
            }
        }
    }

    /**
      * Copy this tensor to a 1D row-major array. The weird syntax of the
      * function signature prevents the array from decaying into a pointer,
      * so this won't compile if you pass the wrong array size.
      */
    void to_array(T (&arr)[N * M]) const {
        std::copy(std::begin(this->data), std::end(this->data), arr);
    }
};

template <class T, size_t I, size_t J, size_t K>
constexpr Tensor<T, I, J> operator*(const Tensor<T, I, K>& t1, const Tensor<T, K, J>& t2) noexcept {
    Tensor<T, I, J> output{};

    for (size_t i = 0; i < I; ++i) {
        for (size_t j = 0; j < J; ++j) {
            for (size_t k = 0; k < K; ++k) {
                output[{i, j}] += t1[{i, k}] * t2[{k, j}];
            }
        }
    }

    return output;
}

template <class T, size_t I, size_t J>
constexpr Tensor<T, I, J> operator+(const Tensor<T, I, J>& t1, const Tensor<T, I, J>& t2) noexcept {
    Tensor<T, I, J> output{};

    for (size_t i = 0; i < I * J; ++i) {
        output.data[i] = t1.data[i] + t2.data[i];
    }

    return output;
}

template <class T, size_t N, size_t M>
Tensor<T, N, M> from_array(const T (&arr)[N][M]) {
    Tensor<T, N, M> out{};

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            out[{i, j}] = arr[i][j];
        }
    }

    return out;
}
