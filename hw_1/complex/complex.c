#include "complex.h"
#include <math.h>

complex add(complex a, complex b)
{
    return (complex){a.real + b.real, a.im + b.im};
}

complex subtract(complex a, complex b)
{
    return (complex){a.real - b.real, a.im - b.im};
}

complex negate(complex a)
{
    return (complex){-a.real, -a.im};
}

complex multiply(complex a, complex b)
{
    return (complex){a.real * b.real - a.im * b.im, a.real * b.im + a.im * b.real};
}

complex divide(complex a, complex b)
{

    if (b.real == 0 && b.im == 0)
    {
        return (complex){INFINITY, INFINITY};
    }

    double denominator = b.real * b.real + b.im * b.im;
    return (complex){(a.real * b.real + a.im * b.im) / denominator, (a.im * b.real - a.real * b.im) / denominator};
}

complex conjugate(complex a)
{
    return (complex){a.real, -a.im};
}

polar to_polar(complex a)
{
    double magnitude = sqrt(a.real * a.real + a.im * a.im);
    return (polar){magnitude, atan2(a.im, a.real)};
}

complex power(complex a, int n)
{
    if (n == 0)
    {
        return (complex){1, 0};
    }
    complex result = {1, 0};
    int abs_n = n > 0 ? n : -n;
    for (int i = 0; i < abs_n; ++i)
    {
        result = multiply(result, a);
    }
    if (n < 0)
    {
        result = divide((complex){1, 0}, result);
    }
    return result;
}

double magnitude(complex a)
{
    return sqrt(a.real * a.real + a.im * a.im);
}

bool equals(complex a, complex b)
{
    return a.real == b.real && a.im == b.im;
}

bool is_zero(complex a)
{
    return a.real == 0 && a.im == 0;
}

bool is_real(complex a)
{
    return a.im == 0;
}

bool is_imaginary(complex a)
{
    return a.real == 0;
}