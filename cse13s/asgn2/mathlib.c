#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#define EPSILON    1E-13 //Used to ensure desired level of accuracy is reached
#define INFLECTION 0.708 //This is the point when x = Sqrt(1 - x * x), used to optimize ArcSin

static inline double Abs(double x) {
    return x > 0 ? x : -x;
}
static inline double Sgn(double x) {
    return x > 0 ? 1 : -1;
}

//Code given in Assignment PDF
double Exp(double x) {
    double term = 1, sum = 1;
    for (int k = 1; Abs(term) > EPSILON; k += 1) {
        term *= x / k;
        sum += term;
    }
    return sum;
}
//Code provided by Professor Darrell Long on Piazza
double Sqrt(double x) {
    double y = 1.0;
    for (double guess = 0.0; Abs(y - guess) > EPSILON; y = (y + x / y) / 2.0) {
        guess = y;
    }
    return y;
}
//Term of Taylor Series is tracked, and multiplied by a multiplying factor to get from one term to
//the next.
//double x: the input
//
//output: the arcSin value of x
double arcSin(double x) {
    if (Abs(x) > INFLECTION) {
        return Sgn(x) * ((.5 * M_PI) - arcSin(Sqrt(1. - x * x))); //ensures desired accuracy.
    }
    double term = x, sum = x;
    for (double i = 1; Abs(term) > EPSILON; i++) {
        term *= .5 * x * x * (2. * i - 1.) * (2. * i - 1.) / (i * (2. * i + 1));
        sum += term;
    }
    return sum;
}
//Used trig identity to compute arcCos
//double x: the input
//
//output: the arcCos value of x
double arcCos(double x) {
    return (.5 * M_PI) - arcSin(x);
}
//Term of Tayler Series is tracked, and multiplied by a multiplying factor to get from one term to
//the next.
//double x: the input
//
//ouput: the artTan value of x
double arcTan(double x) {
    double u = 1. + x * x;
    double term = x / u, sum = x / u;
    for (double i = 1; Abs(term) > EPSILON; i++) {
        term *= (2. * i * x * x) / ((2. * i + 1.) * u);
        sum += term;
    }
    return sum;
}
//Taylor Series does not work here, Newton's method used instead.
//guess is incremented based on slope of function at the guess.
//This is repeated until increment amount has reached desired size.
//double x: the input
//
//output: the Log value of x
double Log(double x) {
    double guess = 1, ex, term;
    do {
        ex = Exp(guess);
        term = (x - ex) / ex;
        guess += term;
    } while (Abs(term) > EPSILON);
    return guess;
}
