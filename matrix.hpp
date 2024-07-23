#ifndef MATRIX_H_
#define MATRIX_H_

#include "utils.hpp"

template <ConvertibleToInt64_t T>
class Matrix {
public:
    std::vector<std::vector<T>> matrix;

private:
    void printForPromptTo(std::ostream &outputStream) const {
        outputStream << "{";
        for (uint64_t i = 0; i < getSize().first; ++i) {
            outputStream << "{";
            for (uint64_t j = 0; j < getSize().second; ++j) {
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
        for (uint64_t i = 0; i < getSize().first; ++i) {
            for (uint64_t j = 0; j < getSize().second; ++j) {
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

        uint64_t rowLength = mat[0].size();
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

    bool isSquareMatrix() const {
        return getSize().first == getSize().second;
    }

    /// Returns the size of the matrix, first is number of rows, second is number of columns.
    /// Matrices with zero rows or columns are not allowed by the constructor, therefore we don't check if matrix[0] exists.
    std::pair<uint64_t, uint64_t> getSize() const { 
        return {matrix.size(), matrix[0].size()};
    }

    bool operator==(const Matrix &other) const {
        if (getSize() != other.getSize()) {
            return false;
        }

        for (uint64_t i = 0; i < getSize().first; ++i) {
            for (uint64_t j = 0; j < getSize().second; ++j) {
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
            case Prompt:
                printForPromptTo(outputStream);
                break;
            case Solution:
                printForSolutionTo(outputStream);
                break;
        }
    }

    static Matrix readMatrix(std::istream &inputStream) {
        std::pair<uint64_t, uint64_t> size;
        inputStream >> size.first >> size.second;
        std::vector<std::vector<T>> matrix(size.first, std::vector<T>(size.second));
        for (uint64_t i = 0; i < size.first; ++i) {
            for (uint64_t j = 0; j < size.second; ++j) {
                inputStream >> matrix[i][j];
            }
        }
        return Matrix(matrix);
    }

    static Matrix constructIdentityMatrix(uint64_t size);

    // Function to get the cofactor matrix (minor matrix)
    Matrix getCofactor(uint64_t delRow, uint64_t delCol) const;

    int64_t getTrace() const;

    int64_t getDeterminant() const;
};

#endif
