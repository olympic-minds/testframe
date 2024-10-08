#include "matrix.hpp"

// Function to get the cofactor matrix (minor matrix)
template <ConvertibleToInt64_t T>
Matrix<T> Matrix<T>::getCofactor(std::uint64_t delRow, std::uint64_t delCol) const {
    std::uint64_t i = 0, j = 0;
    std::vector<std::vector<T>> temp(getSize().first-1, std::vector<T>(getSize().second-1)); 
    for (std::uint64_t row = 0; row < getSize().first; ++row) {
        for (std::uint64_t col = 0; col < getSize().second; ++col) {
            if (row != delRow && col != delCol) {
                temp[i][j++] = matrix[row][col];
                if (j == getSize().second - 1) {
                    j = 0;
                    ++i;
                }
            }
        }
    }

    return temp;
}

template <ConvertibleToInt64_t T>
std::int64_t Matrix<T>::getTrace() const {
    std::int64_t result = 0;
    for (std::uint64_t i = 0; i < getSize().first; ++i) {
        result += matrix[i][i];
    }
    return result;
}

template <ConvertibleToInt64_t T>
std::int64_t Matrix<T>::getDeterminant() const {
    assert(isSquareMatrix());
    
    std::int64_t det = 0;
    if (getSize().first == 1) {
        return matrix[0][0];
    }

    std::int64_t sign = 1;

    for (std::uint64_t f = 0; f < getSize().first; ++f) {
        Matrix<T> temp = getCofactor(matrix, 0, f);
        det += sign * matrix[0][f] * getDeterminant(temp);
        sign = -sign;
    }

    return det;
}
