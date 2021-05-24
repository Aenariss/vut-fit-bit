/* 
*  IZP PROJEKT 2 
*  AUTHOR: Vojtech Fiala
*  LOGIN: xfiala61
*/ 

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define I_0 (1e-12)
#define U_T (25.8563*1e-3)
#define ITERATION_LIMIT 200

//I_p - I_R = 0, 1st Kirchoff's law -> Used to calculate U_p
double func(double U_p, double U_0, double R) {
    double result = (I_0 * (exp(U_p / U_T)-1)) - ((U_0 - U_p) / R);
    return result;
}

//function to calculate U_p through bisection method
double diode(double U_0, double R, double EPS) {
    double min = 0; //minimum interval value
    double max = U_0; //maximum interval value
    double middle = (min+max) / 2; //middle of the interval
    double fmid = func(middle, U_0, R); //middle of the function
    double U_p;
    int i = 0;

        //iterate until you reach the given deviation value epsilon
        while (fabs(max-min) > EPS && i < ITERATION_LIMIT) {
            if (func(min, U_0, R) * fmid < 0) {
                max = middle;
            } 
            else {
                min = middle;
            }
            if (fabs(max-min) > EPS) {
                middle = (min + max) / 2;
                fmid = func(middle, U_0, R);
            }
            i++;
        }
    U_p = middle;
    return U_p;
}

//function to calculate the current I_p
double current(double U_p) {
    double I_p = (I_0 * (exp(U_p / U_T)-1));
    return I_p;
}

int main(int argc, char *argv[]) {
    //check if you have been given enough arguments to work with
    if (argc < 4) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    } //or if you have been given too many
    else if (argc > 4) {
        fprintf(stderr, "Too many arguments\n");
        return 1;
    }
    else {
        char *ptr_1, *ptr_2, *ptr_3; //pointers to be used in the strtod function (point to the string part of the argument)
        double U_0, R, EPS, U_p, I_p;
        U_0 = strtod(argv[1], &ptr_1);
        R = strtod(argv[2], &ptr_2);
        EPS = strtod(argv[3], &ptr_3);

        //Check if the arguments are in a correct format
        if (EPS < 0 || U_0 < 0 || R < 0) { // we cannot have negative values, now can we
            fprintf(stderr, "error: invalid arguments\n");
            return 1;
        }

        U_p = diode(U_0, R, EPS);
        I_p = current(U_p);
        printf("Up=%g V\nIp=%g A\n", U_p, I_p);
        return 0;
    }
}