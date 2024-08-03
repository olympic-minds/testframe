#ifndef MATRIX_H_
#define MATRIX_H_

#include "utils.hpp"
#include <cassert>

enum class MatrixPrintFormat { 
    Prompt,
    Solution
};

template <ConvertibleToInt64_t T>
class Matrix {
public:
    std::vector<std::vector<T>> matrix;

    using PrintFormat = MatrixPrintFormat;
private:
    void printForPromptTo(std::ostream &outputStream) const {
        outputStream << "{";
        for (std::uint64_t i = 0; i < getSize().first; ++i) {
            outputStream << "{";
            for (std::uint64_t j = 0; j < getSize().second; ++j) {
                outputStream << matrix[i][j];
                if (j != getSize().second - 1) {
                    outputStream << ",";
                }
            }
            outputStream << "}";
            if (i != getSize().first - 1) {
                outputStream << ",";
            }
        }
        outputStream << "}\n";
    }

    void printForSolutionTo(std::ostream &outputStream) const {
        outputStream << getSize().first << " " << getSize().second << "\n";
        for (std::uint64_t i = 0; i < getSize().first; ++i) {
            for (std::uint64_t j = 0; j < getSize().second; ++j) {
                outputStream << matrix[i][j];
                if (j != getSize().second - 1) {
                    outputStream << " ";
                }
            }
            outputStream << "\n";
        }
    }

    /// Checks if the vector of vectors is a valid matrix.
    static bool vectorIsValidMatrix(const std::vector<std::vector<T>> &mat) {
        if (mat.size() == 0) {
            return false;
        }

        std::uint64_t rowLength = mat[0].size();
        if (rowLength == 0) {
            return false;
        }

        for (auto row : mat) {
            if (row.size() != rowLength) {
                return false;
            }
        }

        return true;
    }

public:
    Matrix(std::vector<std::vector<T>> mat) {
        assert(vectorIsValidMatrix(mat));

        matrix = mat;
    }

    Matrix(const Matrix<T>& other) {
        matrix = other.matrix;
    }

    bool isSquareMatrix() const {
        return getSize().first == getSize().second;
    }

    /// Returns the size of the matrix, first is number of rows, second is number of columns.
    /// Matrices with zero rows or columns are not allowed by the constructor, therefore we don't check if matrix[0] exists.
    std::pair<std::uint64_t, std::uint64_t> getSize() const { 
        return {matrix.size(), matrix[0].size()};
    }

    bool operator==(const Matrix &other) const {
        if (getSize() != other.getSize()) {
            return false;
        }

        for (std::uint64_t i = 0; i < getSize().first; ++i) {
            for (std::uint64_t j = 0; j < getSize().second; ++j) {
                if (matrix[i][j] != other.matrix[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    operator std::vector<std::vector<T>>() const { return matrix; }

    void printTo(std::ostream &outputStream, PrintFormat format) const {
        switch (format) {
            using enum PrintFormat;
            case Prompt:
                printForPromptTo(outputStream);
                break;
            case Solution:
                printForSolutionTo(outputStream);
                break;
        }
    }

    static Matrix readMatrix(std::istream &inputStream) {
        std::pair<std::uint64_t, std::uint64_t> size;
        inputStream >> size.first >> size.second;
        std::vector<std::vector<T>> matrix(size.first, std::vector<T>(size.second));
        for (std::uint64_t i = 0; i < size.first; ++i) {
            for (std::uint64_t j = 0; j < size.second; ++j) {
                inputStream >> matrix[i][j];
            }
        }
        return Matrix(matrix);
    }

    static Matrix constructIdentityMatrix(std::uint64_t size) {
        std::vector<std::vector<T>> matrix(size, std::vector<T>(size, 0));
        for (std::uint64_t i = 0; i < size; ++i) {
            matrix[i][i] = 1;
        }
        return Matrix(matrix);
    }

    // Function to get the cofactor matrix (minor matrix)
    Matrix getCofactor(std::uint64_t delRow, std::uint64_t delCol) const;

    std::int64_t getTrace() const;

    std::int64_t getDeterminant() const;

    Matrix<T> operator+(const Matrix<T>& other) const {
        if (getSize() != other.getSize()) {
            throw std::invalid_argument("Matrices must have the same dimensions for addition");
        }

        Matrix<T> result(matrix.size(), matrix[0].size());
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        if (matrix[0].size() != other.matrix.size()) {
            throw std::invalid_argument("Matrices must have compatible dimensions for multiplication");
        }

        std::vector<std::vector<T>> result(matrix.size(), std::vector<T>(other.matrix[0].size(), T()));
        for (size_t i = 0; i < result.size(); ++i) {
            for (size_t j = 0; j < result[0].size(); ++j) {
                for (size_t k = 0; k < matrix[0].size(); ++k) {
                    result[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }
        return Matrix(result);
    }

    Matrix<T> operator*(const T& scalar) const {
        Matrix<T> result(matrix.size(), matrix[0].size());
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                result.matrix[i][j] = matrix[i][j] * scalar;
            }
        }
        return result;
    }


    Matrix<T> operator-(const Matrix<T>& other) const {
        if (matrix.size() != other.matrix.size() || matrix[0].size() != other.matrix[0].size()) {
            throw std::invalid_argument("Matrices must have the same dimensions for subtraction");
        }

        Matrix<T> result(matrix.size(), matrix[0].size());
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
            }
        }
        return result;
    }

    Matrix<T> transpose() const {
        Matrix<T> result(matrix[0].size(), matrix.size());
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                result.matrix[j][i] = matrix[i][j];
            }
        }
        return result;
    }

    Matrix<T> pow(std::uint64_t x) const {
        assert(isSquareMatrix());
        
        if(x == 0) {
            return Matrix::constructIdentityMatrix(getSize().first);
        }

        if (x == 1) {
            return Matrix(*this);
        }

        Matrix<T> t = pow(x / 2);

        if(x % 2 == 0) {
            return t * t;
        } else {
            return t * t * Matrix(*this);
        }
    }

    Matrix<T>& operator=(Matrix<T>&& other) noexcept {
        if (this != &other) {
            matrix = std::move(other.matrix);
        }
        return *this;
    }
    
    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this != &other) {
            matrix = other.matrix; 
        }
        return *this;
    }
};

#endif
