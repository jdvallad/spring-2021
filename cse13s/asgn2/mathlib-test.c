#include "mathlib.c"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS   "asctl"
#define STEP_SIZE .1

int main(int argc, char **argv) {
    int opt = 0;
    bool sin, cos, tan, ln;
    sin = cos = tan = ln = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': sin = cos = tan = ln = true; break;
        case 's': sin = true; break;
        case 'c': cos = true; break;
        case 't': tan = true; break;
        case 'l': ln = true; break;
        default: break;
        }
    }
    double x, y;
    if (sin) {
        printf("  x            arcSin           Library        Difference\n");
        printf("  -            ------           -------        ----------\n");
        for (double i = -1.; i < 1.; i += STEP_SIZE) {
            x = arcSin(i);
            y = asin(i);
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, x, y, x - y);
        }
    }
    if (cos) {
        printf("  x            arcCos           Library        Difference\n");
        printf("  -            ------           -------        ----------\n");
        for (double i = -1.; i < 1.; i += STEP_SIZE) {
            x = arcCos(i);
            y = acos(i);
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, x, y, x - y);
        }
    }
    if (tan) {
        printf("  x            arcTan           Library        Difference\n");
        printf("  -            ------           -------        ----------\n");
        for (double i = 1.; i < 10.; i += STEP_SIZE) {
            x = arcTan(i);
            y = atan(i);
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, x, y, x - y);
        }
    }
    if (ln) {
        printf("  x            Log              Library        Difference\n");
        printf("  -            ---              -------        ----------\n");
        for (double i = 1.; i < 10.; i += STEP_SIZE) {
            x = Log(i);
            y = log(i);
            printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", i, x, y, x - y);
        }
    }
    return 0;
}
