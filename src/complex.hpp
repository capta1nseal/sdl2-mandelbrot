#ifndef _MANDELBROTCOMPLEX
#define _MANDELBROTCOMPLEX

struct Complex
{
    double real;
    double imag;

    Complex();
    Complex(double realComponent, double imaginaryComponent);

    void zero();

    void set(double realComponent, double imaginaryComponent);
    void set(Complex other);
    void set(Complex *other);

    void add(Complex other);
    void add(Complex *other);
    Complex addInplace(Complex *other);

    void subtract(Complex other);
    void subtract(Complex *other);
    Complex subtractInplace(Complex *other);

    void multiply(Complex other);
    void multiply(Complex *other);
    Complex multiplyInplace(Complex other);
    Complex multiplyInplace(Complex *other);

    void square();
    Complex squareInplace();

    void scale(double scalar);
    Complex scaleInplace(double scalar);

    double magnitude();
};

Complex addComplex(Complex first, Complex second);
Complex addComplex(Complex first, Complex *second);
Complex addComplex(Complex *first, Complex second);
Complex addComplex(Complex *first, Complex *second);

Complex subtractComplex(Complex first, Complex second);
Complex subtractComplex(Complex first, Complex *second);
Complex subtractComplex(Complex *first, Complex second);
Complex subtractComplex(Complex *first, Complex *second);

Complex scaleComplex(Complex complex, double scalar);
Complex scaleComplex(Complex *complex, double scalar);

#endif
