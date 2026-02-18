#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <cmath>

class Matrix {
private:
    std::vector<double> data_;  // Row-major order storage
    size_t rows_;
    size_t cols_;
    static constexpr double EPSILON = 1e-9;

    // Helper function to calculate index
    size_t index(size_t row, size_t col) const {
        return row * cols_ + col;
    }

public:
    // Constructors
    Matrix();  // Default: 0×0 empty matrix
    Matrix(size_t rows, size_t cols);  // Zero-initialized matrix
    Matrix(size_t rows, size_t cols, double value);  // Matrix filled with value
    Matrix(std::initializer_list<std::initializer_list<double>> list);  // From 2D initializer list
    Matrix(const Matrix& other);  // Copy constructor

    // Assignment operator
    Matrix& operator=(const Matrix& other);

    // Element access
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;
    double& at(size_t row, size_t col);
    const double& at(size_t row, size_t col) const;

    // Size and properties
    size_t rows() const;
    size_t cols() const;
    bool isEmpty() const;
    bool isSquare() const;

    // Arithmetic operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    friend Matrix operator*(double scalar, const Matrix& m);
    Matrix operator/(double scalar) const;
    Matrix operator-() const;  // Unary negation

    // Compound assignment operators
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator*=(double scalar);
    Matrix& operator/=(double scalar);

    // Comparison operators
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    // Matrix operations
    Matrix transpose() const;
    double trace() const;
    Matrix diagonal() const;  // Extract diagonal as column vector
    void fill(double value);
    double norm() const;  // Frobenius norm

    // Static factory methods
    static Matrix identity(size_t n);
    static Matrix zeros(size_t rows, size_t cols);
    static Matrix ones(size_t rows, size_t cols);
    static Matrix diagonal(const std::vector<double>& diag);
};

#endif
