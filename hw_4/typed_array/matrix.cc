#include "matrix.h"
#include <algorithm>
#include <numeric>

// Default constructor: 0×0 empty matrix
Matrix::Matrix() : rows_(0), cols_(0) {
}

// Zero-initialized matrix
Matrix::Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
    data_.resize(rows * cols, 0.0);
}

// Matrix filled with specified value
Matrix::Matrix(size_t rows, size_t cols, double value) : rows_(rows), cols_(cols) {
    data_.resize(rows * cols, value);
}

// Construct from 2D initializer list
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) {
    if (list.size() == 0) {
        rows_ = 0;
        cols_ = 0;
        return;
    }
    
    rows_ = list.size();
    cols_ = list.begin()->size();
    
    // Check that all rows have the same size
    for (const auto& row : list) {
        if (row.size() != cols_) {
            throw std::invalid_argument("All rows must have the same size");
        }
    }
    
    data_.reserve(rows_ * cols_);
    for (const auto& row : list) {
        for (double val : row) {
            data_.push_back(val);
        }
    }
}

// Copy constructor
Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {
}

// Assignment operator
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
    }
    return *this;
}

// Element access - non-const
double& Matrix::operator()(size_t row, size_t col) {
    return data_[index(row, col)];
}

// Element access - const
const double& Matrix::operator()(size_t row, size_t col) const {
    return data_[index(row, col)];
}

// Element access with bounds checking - non-const
double& Matrix::at(size_t row, size_t col) {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data_[index(row, col)];
}

// Element access with bounds checking - const
const double& Matrix::at(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data_[index(row, col)];
}

// Size and properties
size_t Matrix::rows() const {
    return rows_;
}

size_t Matrix::cols() const {
    return cols_;
}

bool Matrix::isEmpty() const {
    return rows_ == 0 || cols_ == 0;
}

bool Matrix::isSquare() const {
    return rows_ == cols_ && rows_ > 0;
}

// Arithmetic operators
Matrix Matrix::operator+(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    Matrix result(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) {
        result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    Matrix result(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) {
        result.data_[i] = data_[i] - other.data_[i];
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    Matrix result(rows_, other.cols_);
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < other.cols_; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < cols_; k++) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows_, cols_);
    for (size_t i = 0; i < data_.size(); i++) {
        result.data_[i] = data_[i] * scalar;
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& m) {
    return m * scalar;
}

Matrix Matrix::operator/(double scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::invalid_argument("Division by zero");
    }
    return *this * (1.0 / scalar);
}

Matrix Matrix::operator-() const {
    return *this * -1.0;
}

// Compound assignment operators
Matrix& Matrix::operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    *this = *this * other;
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0; i < data_.size(); i++) {
        data_[i] *= scalar;
    }
    return *this;
}

Matrix& Matrix::operator/=(double scalar) {
    if (std::abs(scalar) < EPSILON) {
        throw std::invalid_argument("Division by zero");
    }
    *this *= (1.0 / scalar);
    return *this;
}

// Comparison operators
bool Matrix::operator==(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    for (size_t i = 0; i < data_.size(); i++) {
        if (std::abs(data_[i] - other.data_[i]) >= EPSILON) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

// Matrix operations
Matrix Matrix::transpose() const {
    Matrix result(cols_, rows_);
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

double Matrix::trace() const {
    if (!isSquare()) {
        throw std::logic_error("Trace is only defined for square matrices");
    }
    
    double sum = 0.0;
    for (size_t i = 0; i < rows_; i++) {
        sum += (*this)(i, i);
    }
    return sum;
}

Matrix Matrix::diagonal() const {
    if (!isSquare()) {
        throw std::logic_error("Diagonal extraction is only defined for square matrices");
    }
    
    Matrix result(rows_, 1);
    for (size_t i = 0; i < rows_; i++) {
        result(i, 0) = (*this)(i, i);
    }
    return result;
}

void Matrix::fill(double value) {
    std::fill(data_.begin(), data_.end(), value);
}

double Matrix::norm() const {
    double sum = 0.0;
    for (double val : data_) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

// Static factory methods
Matrix Matrix::identity(size_t n) {
    Matrix result(n, n);
    for (size_t i = 0; i < n; i++) {
        result(i, i) = 1.0;
    }
    return result;
}

Matrix Matrix::zeros(size_t rows, size_t cols) {
    return Matrix(rows, cols, 0.0);
}

Matrix Matrix::ones(size_t rows, size_t cols) {
    return Matrix(rows, cols, 1.0);
}

Matrix Matrix::diagonal(const std::vector<double>& diag) {
    size_t n = diag.size();
    Matrix result(n, n, 0.0);
    for (size_t i = 0; i < n; i++) {
        result(i, i) = diag[i];
    }
    return result;
}
