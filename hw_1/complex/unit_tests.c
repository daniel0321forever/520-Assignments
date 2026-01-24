#include "complex.h"
#include "gtest/gtest.h"

namespace
{

    TEST(Complex, Add)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){4, 5},
                c = (complex){0, 0};
        EXPECT_DOUBLE_EQ(add(a, b).real, 6);
        EXPECT_DOUBLE_EQ(add(a, b).im, 8);
    }

    TEST(Complex, Subtract)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){4, 5},
                c = (complex){0, 0};
        EXPECT_DOUBLE_EQ(subtract(a, b).real, -2);
        EXPECT_DOUBLE_EQ(subtract(a, b).im, -2);
    }

    TEST(Complex, Negate)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){-2, -3};
        EXPECT_DOUBLE_EQ(negate(a).real, -2);
        EXPECT_DOUBLE_EQ(negate(a).im, -3);
    }

    TEST(Complex, Multiply)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){4, 5},
                c = (complex){0, 0};
        EXPECT_DOUBLE_EQ(multiply(a, b).real, -7);
        EXPECT_DOUBLE_EQ(multiply(a, b).im, 22);
    }

    TEST(Complex, Divide)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){4, 5},
                c = (complex){0, 0};

        EXPECT_NEAR(divide(a, b).real, 0.56097561, 1e-6);
        EXPECT_NEAR(divide(a, b).im, 0.04878, 1e-5);
        EXPECT_EQ(divide(a, c).real, INFINITY);
        EXPECT_EQ(divide(a, c).im, INFINITY);
    }

    TEST(Complex, Conjugate)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_DOUBLE_EQ(conjugate(a).real, 2);
        EXPECT_DOUBLE_EQ(conjugate(a).im, -3);
    }

    TEST(Complex, To_Polar)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_NEAR(to_polar(a).r, 3.60555, 1e-5);
        EXPECT_NEAR(to_polar(a).theta, 0.982794, 1e-5);
    }

    TEST(Complex, Power)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3};
        EXPECT_NEAR(power(a, 2).real, -5, 1e-5);
        EXPECT_NEAR(power(a, 2).im, 12, 1e-5);
        EXPECT_NEAR(power(a, -2).real, -0.0295857988, 1e-5);
        EXPECT_NEAR(power(a, -2).im, -0.0710059172, 1e-5);
    }

    TEST(Complex, Magnitude)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_NEAR(magnitude(a), 3.60555, 1e-5);
    }

    TEST(Complex, Equals)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_EQ(equals(a, b), false);
    }

    TEST(Complex, Is_Zero)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_EQ(is_zero(a), false);
    }

    TEST(Complex, Is_Real)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_EQ(is_real(a), false);
    }

    TEST(Complex, Is_Imaginary)
    {
        EXPECT_TRUE(true);
        complex a = (complex){2, 3},
                b = (complex){2, -3};
        EXPECT_EQ(is_imaginary(a), false);
    }
}
