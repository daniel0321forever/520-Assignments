#include <math.h>
#include <float.h> /* defines DBL_EPSILON */
#include <assert.h>
#include "typed_array.h"
#include "point.h"
#include "matrix.h"
#include "gtest/gtest.h"

namespace
{

    TEST(TypedArray, Construction)
    {
        TypedArray<Point> b;
        b.set(0, Point(1, 2, 3));
        b.set(1, Point(2, 3, 4));
        b.set(20, Point(3, 4, 5));
        EXPECT_EQ(b.get(0).x, 1);
        EXPECT_EQ(b.get(1).y, 3);
        EXPECT_EQ(b.get(20).z, 5);
    }

    TEST(TypedArray, Defaults)
    {
        TypedArray<Point> x;
        Point &y = x.get(3);
        std::cout << x << "\n";
        EXPECT_DOUBLE_EQ(y.magnitude(), 0.0);
    }

    TEST(TypedArray, Matrix)
    {

        TypedArray<TypedArray<double>> m;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                m.get(i).set(j, 3 * i + j);
            }
        }

        std::cout << m << "\n";

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                EXPECT_DOUBLE_EQ(m.get(i).get(j), 3 * i + j);
            }
        }
    }

    TEST(TypedArray, CopyElementsInSet1)
    {
        TypedArray<Point> b;
        Point p(1, 2, 3);
        b.set(0, p);
        p.x = 4;
        EXPECT_DOUBLE_EQ(b.get(0).x, 1);
    }

    TEST(TypedArray, CopyElementsInSet2)
    {
        TypedArray<TypedArray<double>> m;
        TypedArray<double> x;
        x.set(0, 0);
        m.set(0, x);
        x.set(0, -1);
        EXPECT_DOUBLE_EQ(m.get(0).get(0), 0.0); // If set didn't make a copy
                                                // then we would expect m[0][0]
                                                // to be x[0], which we changed
                                                // to -1.
    }

    // Tests for push, pop, push_front, pop_front
    TEST(TypedArray, PushPop)
    {
        TypedArray<int> a;
        a.push(1);
        a.push(2);
        a.push(3);
        EXPECT_EQ(a.size(), 3);
        EXPECT_EQ(a.get(0), 1);
        EXPECT_EQ(a.get(1), 2);
        EXPECT_EQ(a.get(2), 3);

        int val = a.pop();
        EXPECT_EQ(val, 3);
        EXPECT_EQ(a.size(), 2);
        EXPECT_EQ(a.get(0), 1);
        EXPECT_EQ(a.get(1), 2);

        a.pop();
        a.pop();
        EXPECT_EQ(a.size(), 0);

        EXPECT_THROW(a.pop(), std::range_error);
    }

    TEST(TypedArray, PushFrontPopFront)
    {
        TypedArray<int> a;
        a.push_front(3);
        a.push_front(2);
        a.push_front(1);
        EXPECT_EQ(a.size(), 3);
        EXPECT_EQ(a.get(0), 1);
        EXPECT_EQ(a.get(1), 2);
        EXPECT_EQ(a.get(2), 3);

        int val = a.pop_front();
        EXPECT_EQ(val, 1);
        EXPECT_EQ(a.size(), 2);
        EXPECT_EQ(a.get(0), 2);
        EXPECT_EQ(a.get(1), 3);

        a.pop_front();
        a.pop_front();
        EXPECT_EQ(a.size(), 0);

        EXPECT_THROW(a.pop_front(), std::range_error);
    }

    TEST(TypedArray, Concat)
    {
        TypedArray<int> a;
        a.set(0, 0);
        a.set(1, 1);

        TypedArray<int> b = a.concat(a).concat(a);
        EXPECT_EQ(b.size(), 6);
        EXPECT_EQ(b.get(0), 0);
        EXPECT_EQ(b.get(1), 1);
        EXPECT_EQ(b.get(2), 0);
        EXPECT_EQ(b.get(3), 1);
        EXPECT_EQ(b.get(4), 0);
        EXPECT_EQ(b.get(5), 1);

        // Verify original array is untouched
        EXPECT_EQ(a.size(), 2);
        EXPECT_EQ(a.get(0), 0);
        EXPECT_EQ(a.get(1), 1);
    }

    TEST(TypedArray, Reverse)
    {
        TypedArray<int> a;
        a.push(1);
        a.push(2);
        a.push(3);
        a.push(4);

        TypedArray<int> &ref = a.reverse();
        EXPECT_EQ(&ref, &a); // Should return reference to self

        EXPECT_EQ(a.get(0), 4);
        EXPECT_EQ(a.get(1), 3);
        EXPECT_EQ(a.get(2), 2);
        EXPECT_EQ(a.get(3), 1);

        // Reverse again should restore original
        a.reverse();
        EXPECT_EQ(a.get(0), 1);
        EXPECT_EQ(a.get(1), 2);
        EXPECT_EQ(a.get(2), 3);
        EXPECT_EQ(a.get(3), 4);
    }

    TEST(TypedArray, OperatorPlus)
    {
        TypedArray<int> a;
        a.set(0, 0);
        a.set(1, 1);

        TypedArray<int> b = a + a + a;
        EXPECT_EQ(b.size(), 6);
        EXPECT_EQ(b.get(0), 0);
        EXPECT_EQ(b.get(1), 1);
        EXPECT_EQ(b.get(2), 0);
        EXPECT_EQ(b.get(3), 1);
        EXPECT_EQ(b.get(4), 0);
        EXPECT_EQ(b.get(5), 1);

        // Verify original array is untouched
        EXPECT_EQ(a.size(), 2);
        EXPECT_EQ(a.get(0), 0);
        EXPECT_EQ(a.get(1), 1);
    }

    TEST(TypedArray, EmptyArrayOperations)
    {
        TypedArray<int> a;
        EXPECT_EQ(a.size(), 0);
        EXPECT_THROW(a.pop(), std::range_error);
        EXPECT_THROW(a.pop_front(), std::range_error);

        // Empty concat should work
        TypedArray<int> b = a.concat(a);
        EXPECT_EQ(b.size(), 0);
    }

    // Matrix tests
    TEST(Matrix, DefaultConstructor)
    {
        Matrix m;
        EXPECT_EQ(m.rows(), 0);
        EXPECT_EQ(m.cols(), 0);
        EXPECT_TRUE(m.isEmpty());
    }

    TEST(Matrix, SizedConstructor)
    {
        Matrix m(3, 4);
        EXPECT_EQ(m.rows(), 3);
        EXPECT_EQ(m.cols(), 4);
        EXPECT_FALSE(m.isEmpty());
        EXPECT_FALSE(m.isSquare());

        // All elements should be zero
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                EXPECT_DOUBLE_EQ(m(i, j), 0.0);
            }
        }
    }

    TEST(Matrix, ValueConstructor)
    {
        Matrix m(2, 3, 5.5);
        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 3);
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                EXPECT_DOUBLE_EQ(m(i, j), 5.5);
            }
        }
    }

    TEST(Matrix, InitializerListConstructor)
    {
        Matrix m = {{1, 2}, {3, 4}};
        EXPECT_EQ(m.rows(), 2);
        EXPECT_EQ(m.cols(), 2);
        EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
        EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
        EXPECT_DOUBLE_EQ(m(1, 1), 4.0);
        EXPECT_TRUE(m.isSquare());
    }

    TEST(Matrix, InitializerListUnequalRows)
    {
        EXPECT_THROW(Matrix({{1, 2}, {3}}), std::invalid_argument);
    }

    TEST(Matrix, CopyConstructor)
    {
        Matrix m1 = {{1, 2}, {3, 4}};
        Matrix m2(m1);

        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 2);
        EXPECT_DOUBLE_EQ(m2(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(m2(1, 1), 4.0);

        m2(0, 0) = 99.0;
        EXPECT_DOUBLE_EQ(m1(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(m2(0, 0), 99.0);
    }

    TEST(Matrix, AssignmentOperator)
    {
        Matrix m1 = {{1, 2}, {3, 4}};
        Matrix m2;
        m2 = m1;

        EXPECT_EQ(m2.rows(), 2);
        EXPECT_EQ(m2.cols(), 2);
        EXPECT_DOUBLE_EQ(m2(0, 0), 1.0);

        m2(0, 0) = 99.0;
        EXPECT_DOUBLE_EQ(m1(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(m2(0, 0), 99.0);
    }

    TEST(Matrix, ElementAccess)
    {
        Matrix m = {{1, 2}, {3, 4}};

        m(0, 0) = 10.0;
        EXPECT_DOUBLE_EQ(m(0, 0), 10.0);

        // Const access
        const Matrix &cm = m;
        EXPECT_DOUBLE_EQ(cm(1, 1), 4.0);
    }

    TEST(Matrix, AtWithBoundsChecking)
    {
        Matrix m = {{1, 2}, {3, 4}};

        EXPECT_DOUBLE_EQ(m.at(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(m.at(1, 1), 4.0);

        EXPECT_THROW(m.at(2, 0), std::out_of_range);
        EXPECT_THROW(m.at(0, 2), std::out_of_range);
        EXPECT_THROW(m.at(10, 10), std::out_of_range);
    }

    TEST(Matrix, IsSquare)
    {
        Matrix m1(3, 3);
        EXPECT_TRUE(m1.isSquare());

        Matrix m2(3, 4);
        EXPECT_FALSE(m2.isSquare());

        Matrix m3;
        EXPECT_FALSE(m3.isSquare());
    }

    TEST(Matrix, Addition)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{5, 6}, {7, 8}};
        Matrix C = A + B;

        EXPECT_DOUBLE_EQ(C(0, 0), 6.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 8.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 10.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 12.0);

        // Original matrices unchanged
        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(B(0, 0), 5.0);
    }

    TEST(Matrix, AdditionDimensionMismatch)
    {
        Matrix A(2, 3);
        Matrix B(3, 2);
        EXPECT_THROW(A + B, std::invalid_argument);
    }

    TEST(Matrix, Subtraction)
    {
        Matrix A = {{5, 6}, {7, 8}};
        Matrix B = {{1, 2}, {3, 4}};
        Matrix C = A - B;

        EXPECT_DOUBLE_EQ(C(0, 0), 4.0);
        EXPECT_DOUBLE_EQ(C(0, 1), 4.0);
        EXPECT_DOUBLE_EQ(C(1, 0), 4.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 4.0);
    }

    TEST(Matrix, Multiplication)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{5, 6}, {7, 8}};
        Matrix C = A * B;

        EXPECT_DOUBLE_EQ(C(0, 0), 19.0); // 1*5 + 2*7
        EXPECT_DOUBLE_EQ(C(0, 1), 22.0); // 1*6 + 2*8
        EXPECT_DOUBLE_EQ(C(1, 0), 43.0); // 3*5 + 4*7
        EXPECT_DOUBLE_EQ(C(1, 1), 50.0); // 3*6 + 4*8
    }

    TEST(Matrix, MultiplicationDimensionMismatch)
    {
        Matrix A(2, 3);
        Matrix B(2, 3);
        EXPECT_THROW(A * B, std::invalid_argument);
    }

    TEST(Matrix, ScalarMultiplication)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = A * 2.0;

        EXPECT_DOUBLE_EQ(B(0, 0), 2.0);
        EXPECT_DOUBLE_EQ(B(0, 1), 4.0);
        EXPECT_DOUBLE_EQ(B(1, 0), 6.0);
        EXPECT_DOUBLE_EQ(B(1, 1), 8.0);

        // Friend function
        Matrix C = 3.0 * A;
        EXPECT_DOUBLE_EQ(C(0, 0), 3.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 12.0);
    }

    TEST(Matrix, ScalarDivision)
    {
        Matrix A = {{2, 4}, {6, 8}};
        Matrix B = A / 2.0;

        EXPECT_DOUBLE_EQ(B(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(B(0, 1), 2.0);
        EXPECT_DOUBLE_EQ(B(1, 0), 3.0);
        EXPECT_DOUBLE_EQ(B(1, 1), 4.0);

        EXPECT_THROW(A / 0.0, std::invalid_argument);
    }

    TEST(Matrix, UnaryNegation)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = -A;

        EXPECT_DOUBLE_EQ(B(0, 0), -1.0);
        EXPECT_DOUBLE_EQ(B(0, 1), -2.0);
        EXPECT_DOUBLE_EQ(B(1, 0), -3.0);
        EXPECT_DOUBLE_EQ(B(1, 1), -4.0);
    }

    TEST(Matrix, CompoundAssignment)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{1, 1}, {1, 1}};

        A += B;
        EXPECT_DOUBLE_EQ(A(0, 0), 2.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 5.0);

        A -= B;
        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 4.0);

        A *= 2.0;
        EXPECT_DOUBLE_EQ(A(0, 0), 2.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 8.0);

        A /= 2.0;
        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 4.0);
    }

    TEST(Matrix, MatrixMultiplicationAssignment)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{1, 0}, {0, 1}};
        A *= B;

        EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(A(0, 1), 2.0);
        EXPECT_DOUBLE_EQ(A(1, 0), 3.0);
        EXPECT_DOUBLE_EQ(A(1, 1), 4.0);
    }

    TEST(Matrix, Equality)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{1, 2}, {3, 4}};
        Matrix C = {{1, 2}, {3, 5}};

        EXPECT_TRUE(A == B);
        EXPECT_FALSE(A == C);
        EXPECT_TRUE(A != C);
        EXPECT_FALSE(A != B);
    }

    TEST(Matrix, Transpose)
    {
        Matrix A = {{1, 2, 3}, {4, 5, 6}};
        Matrix B = A.transpose();

        EXPECT_EQ(B.rows(), 3);
        EXPECT_EQ(B.cols(), 2);
        EXPECT_DOUBLE_EQ(B(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(B(0, 1), 4.0);
        EXPECT_DOUBLE_EQ(B(1, 0), 2.0);
        EXPECT_DOUBLE_EQ(B(1, 1), 5.0);
        EXPECT_DOUBLE_EQ(B(2, 0), 3.0);
        EXPECT_DOUBLE_EQ(B(2, 1), 6.0);

        // (A^T)^T = A
        Matrix C = B.transpose();
        EXPECT_TRUE(C == A);
    }

    TEST(Matrix, Trace)
    {
        Matrix A = {{1, 2}, {3, 4}};
        double tr = A.trace();
        EXPECT_DOUBLE_EQ(tr, 5.0); // 1 + 4

        Matrix B(3, 4);
        EXPECT_THROW(B.trace(), std::logic_error);
    }

    TEST(Matrix, DiagonalExtraction)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix diag = A.diagonal();

        EXPECT_EQ(diag.rows(), 2);
        EXPECT_EQ(diag.cols(), 1);
        EXPECT_DOUBLE_EQ(diag(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(diag(1, 0), 4.0);

        Matrix B(3, 4);
        EXPECT_THROW(B.diagonal(), std::logic_error);
    }

    TEST(Matrix, Fill)
    {
        Matrix A(2, 3);
        A.fill(7.5);

        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                EXPECT_DOUBLE_EQ(A(i, j), 7.5);
            }
        }
    }

    TEST(Matrix, Norm)
    {
        Matrix A = {{3, 4}};
        double n = A.norm();
        EXPECT_DOUBLE_EQ(n, 5.0); // sqrt(3^2 + 4^2) = 5

        Matrix B = {{1, 2}, {3, 4}};
        double n2 = B.norm();
        double expected = std::sqrt(1 * 1 + 2 * 2 + 3 * 3 + 4 * 4);
        EXPECT_DOUBLE_EQ(n2, expected);
    }

    TEST(Matrix, Identity)
    {
        Matrix I = Matrix::identity(3);

        EXPECT_EQ(I.rows(), 3);
        EXPECT_EQ(I.cols(), 3);
        EXPECT_TRUE(I.isSquare());

        for (size_t i = 0; i < 3; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                if (i == j)
                {
                    EXPECT_DOUBLE_EQ(I(i, j), 1.0);
                }
                else
                {
                    EXPECT_DOUBLE_EQ(I(i, j), 0.0);
                }
            }
        }

        // trace(I_n) = n
        EXPECT_DOUBLE_EQ(I.trace(), 3.0);
    }

    TEST(Matrix, Zeros)
    {
        Matrix Z = Matrix::zeros(2, 3);

        EXPECT_EQ(Z.rows(), 2);
        EXPECT_EQ(Z.cols(), 3);
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                EXPECT_DOUBLE_EQ(Z(i, j), 0.0);
            }
        }
    }

    TEST(Matrix, Ones)
    {
        Matrix O = Matrix::ones(2, 3);

        EXPECT_EQ(O.rows(), 2);
        EXPECT_EQ(O.cols(), 3);
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                EXPECT_DOUBLE_EQ(O(i, j), 1.0);
            }
        }
    }

    TEST(Matrix, DiagonalFactory)
    {
        std::vector<double> diag = {1.0, 2.0, 3.0};
        Matrix D = Matrix::diagonal(diag);

        EXPECT_EQ(D.rows(), 3);
        EXPECT_EQ(D.cols(), 3);
        EXPECT_DOUBLE_EQ(D(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(D(1, 1), 2.0);
        EXPECT_DOUBLE_EQ(D(2, 2), 3.0);
        EXPECT_DOUBLE_EQ(D(0, 1), 0.0);
        EXPECT_DOUBLE_EQ(D(1, 0), 0.0);
    }

    TEST(Matrix, EdgeCases)
    {
        // 1x1 matrix
        Matrix m1(1, 1, 5.0);
        EXPECT_TRUE(m1.isSquare());
        EXPECT_DOUBLE_EQ(m1(0, 0), 5.0);
        EXPECT_DOUBLE_EQ(m1.trace(), 5.0);

        // Empty matrix operations
        Matrix empty;
        EXPECT_TRUE(empty.isEmpty());
        EXPECT_FALSE(empty.isSquare());

        // A + 0 = A
        Matrix A = {{1, 2}, {3, 4}};
        Matrix Z = Matrix::zeros(2, 2);
        Matrix result = A + Z;
        EXPECT_TRUE(result == A);
    }

    TEST(Matrix, ExampleUsage)
    {
        Matrix A = {{1, 2}, {3, 4}};
        Matrix B = {{5, 6}, {7, 8}};

        Matrix C = A + B;
        EXPECT_DOUBLE_EQ(C(0, 0), 6.0);
        EXPECT_DOUBLE_EQ(C(1, 1), 12.0);

        Matrix D = A * B;
        EXPECT_DOUBLE_EQ(D(0, 0), 19.0);

        Matrix E = 2.0 * A;
        EXPECT_DOUBLE_EQ(E(0, 0), 2.0);

        Matrix F = A.transpose();
        EXPECT_DOUBLE_EQ(F(0, 0), 1.0);
        EXPECT_DOUBLE_EQ(F(0, 1), 3.0);

        double tr = A.trace();
        EXPECT_DOUBLE_EQ(tr, 5.0);

        double n = A.norm();
        double expected = std::sqrt(1 * 1 + 2 * 2 + 3 * 3 + 4 * 4);
        EXPECT_DOUBLE_EQ(n, expected);
    }

}