#include "complex.hpp"

#include <cmath>

Complex::Complex() {
    real = 0.0;
    imag = 0.0;
}

Complex::Complex(double initReal, double initImag)
    : real(initReal), imag(initImag) {}

void Complex::zero() {
    real = 0.0;
    imag = 0.0;
}

void Complex::set(double initReal, double initImag) {
    real = initReal;
    imag = initImag;
}
void Complex::set(Complex other) {
    real = other.real;
    imag = other.imag;
}
void Complex::set(Complex *other) {
    real = other->real;
    imag = other->imag;
}

void Complex::add(Complex other) {
    real += other.real;
    imag += other.imag;
}
void Complex::add(Complex *other) {
    real += other->real;
    imag += other->imag;
}
Complex Complex::addInplace(Complex *other) {
    return Complex(real + other->real, imag + other->imag);
}

void Complex::subtract(Complex other) {
    real -= other.real;
    imag -= other.imag;
}
void Complex::subtract(Complex *other) {
    real -= other->real;
    imag -= other->imag;
}
Complex Complex::subtractInplace(Complex *other) {
    return Complex(real - other->real, imag - other->imag);
}

void Complex::multiply(Complex other) {
    set(real * other.real - imag * other.imag,
        real * other.imag + imag * other.real);
}
void Complex::multiply(Complex *other) {
    set(real * other->real - imag * other->imag,
        real * other->imag + imag * other->real);
}
Complex Complex::multiplyInplace(Complex other) {
    return Complex(real * other.real - imag * other.imag,
                   real * other.imag + imag * other.real);
}
Complex Complex::multiplyInplace(Complex *other) {
    return Complex(real * other->real - imag * other->imag,
                   real * other->imag + imag * other->real);
}

void Complex::square() { multiply(Complex(real, imag)); }
Complex Complex::squareInplace() {
    return multiplyInplace(Complex(real, imag));
}

void Complex::squaredPlus(Complex other) {
    double realSquared = real * real;
    double imagSquared = imag * imag;
    imag = (real + real) * imag + other.imag;
    real = realSquared - imagSquared + other.real;
}

void Complex::scale(double scalar) {
    real *= scalar;
    imag *= scalar;
}
Complex Complex::scaleInplace(double scalar) {
    return Complex(real * scalar, imag * scalar);
}

double Complex::magnitude() { return std::sqrt(real * real + imag * imag); }
double Complex::magnitudeSquared() { return real * real + imag * imag; }

Complex addComplex(Complex first, Complex second) {
    return Complex(first.real + second.real, first.imag + second.imag);
}
Complex addComplex(Complex first, Complex *second) {
    return Complex(first.real + second->real, first.imag + second->imag);
}
Complex addComplex(Complex *first, Complex second) {
    return Complex(first->real + second.real, first->imag + second.imag);
}
Complex addComplex(Complex *first, Complex *second) {
    return Complex(first->real + second->real, first->imag + second->imag);
}

Complex subtractComplex(Complex first, Complex second) {
    return Complex(first.real - second.real, first.imag - second.imag);
}
Complex subtractComplex(Complex first, Complex *second) {
    return Complex(first.real - second->real, first.imag - second->imag);
}
Complex subtractComplex(Complex *first, Complex second) {
    return Complex(first->real - second.real, first->imag - second.imag);
}
Complex subtractComplex(Complex *first, Complex *second) {
    return Complex(first->real - second->real, first->imag - second->imag);
}

Complex scaleComplex(Complex complex, double scalar) {
    return Complex(complex.real * scalar, complex.imag * scalar);
}
Complex scaleComplex(Complex *complex, double scalar) {
    return Complex(complex->real * scalar, complex->imag * scalar);
}
