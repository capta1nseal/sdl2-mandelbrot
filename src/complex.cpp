#include "complex.hpp"

#include <cmath>

Complex::Complex()
{
    real = 0.0;
    imaginary = 0.0;
}

Complex::Complex(double realComponent, double imaginaryComponent)
    : real(realComponent), imaginary(imaginaryComponent)
{
}

void Complex::zero()
{
    real = 0.0;
    imaginary = 0.0;
}

void Complex::set(double realComponent, double imaginaryComponent)
{
    real = realComponent;
    imaginary = imaginaryComponent;
}
void Complex::set(Complex other)
{
    real = other.real;
    imaginary = other.imaginary;
}
void Complex::set(Complex *other)
{
    real = other->real;
    imaginary = other->imaginary;
}

void Complex::add(Complex other)
{
    real += other.real;
    imaginary += other.imaginary;
}
void Complex::add(Complex *other)
{
    real += other->real;
    imaginary += other->imaginary;
}
Complex Complex::addInplace(Complex *other)
{
    return Complex(real + other->real, imaginary + other->imaginary);
}

void Complex::subtract(Complex other)
{
    real -= other.real;
    imaginary -= other.imaginary;
}
void Complex::subtract(Complex *other)
{
    real -= other->real;
    imaginary -= other->imaginary;
}
Complex Complex::subtractInplace(Complex *other)
{
    return Complex(real - other->real, imaginary - other->imaginary);
}

void Complex::multiply(Complex other)
{
    set(
        real * other.real - imaginary * other.imaginary,
        real * other.imaginary + imaginary * other.real
    );
}
void Complex::multiply(Complex *other)
{
    set(
        real * other->real - imaginary * other->imaginary,
        real * other->imaginary + imaginary * other->real
    );
}
Complex Complex::multiplyInplace(Complex other)
{
    return Complex(
        real * other.real - imaginary * other.imaginary,
        real * other.imaginary + imaginary * other.real
    );
}
Complex Complex::multiplyInplace(Complex *other)
{
    return Complex(
        real * other->real - imaginary * other->imaginary,
        real * other->imaginary + imaginary * other->real
    );
}

void Complex::square()
{
    multiply(Complex(real, imaginary));
}
Complex Complex::squareInplace()
{
    return multiplyInplace(Complex(real, imaginary));
}

void Complex::scale(double scalar)
{
    real *= scalar;
    imaginary *= scalar;
}
Complex Complex::scaleInplace(double scalar)
{
    return Complex(real * scalar, imaginary * scalar);
}

double Complex::magnitude()
{
    return std::sqrt(real * real + imaginary * imaginary);
}


Complex addComplex(Complex first, Complex second) { return Complex(first.real + second.real, first.imaginary + second.imaginary); }
Complex addComplex(Complex first, Complex *second) { return Complex(first.real + second->real, first.imaginary + second->imaginary); }
Complex addComplex(Complex *first, Complex second) { return Complex(first->real + second.real, first->imaginary + second.imaginary); }
Complex addComplex(Complex *first, Complex *second) { return Complex(first->real + second->real, first->imaginary + second->imaginary); }

Complex subtractComplex(Complex first, Complex second) { return Complex(first.real - second.real, first.imaginary - second.imaginary); }
Complex subtractComplex(Complex first, Complex *second) { return Complex(first.real - second->real, first.imaginary - second->imaginary); }
Complex subtractComplex(Complex *first, Complex second) { return Complex(first->real - second.real, first->imaginary - second.imaginary); }
Complex subtractComplex(Complex *first, Complex *second) { return Complex(first->real - second->real, first->imaginary - second->imaginary); }

Complex scaleComplex(Complex complex, double scalar) { return Complex(complex.real * scalar, complex.imaginary * scalar); }
Complex scaleComplex(Complex *complex, double scalar) { return Complex(complex->real * scalar, complex->imaginary * scalar); }
