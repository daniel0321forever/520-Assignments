#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdbool.h>

typedef struct
{
    double real;
    double im;
} complex;

typedef struct
{
    double r;
    double theta;
} polar;

complex add(complex a, complex b);
complex subtract(complex a, complex b);
complex negate(complex a);
complex multiply(complex a, complex b);
complex divide(complex a, complex b);
complex conjugate(complex a);
polar to_polar(complex a);
complex power(complex a, int n);
double magnitude(complex a);

bool equals(complex a, complex b);
bool is_zero(complex a);
bool is_real(complex a);
bool is_imaginary(complex a);

#endif
