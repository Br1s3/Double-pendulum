#ifndef SIMULTAION_NUMERIQUE_H_INCLUED
#define SIMULTAION_NUMERIQUE_H_INCLUED

#define ABS(x) ((x < 0) ? -(x) : (x))

int methode_euler_explicite(const double dt, double t, double *x, double *v, double (*f)(double, double, double));
// int methode_euler_simpletique(const double dt, double *x, double *v, double *a);
int methode_euler_simpletique(const double dt, double t, double *x, double *v, double (*f)(double, double, double));

int methode_RK4(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int methode_RK(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int methode_Verlet(const double h, double t, double *x, double *v, double (*f)(double, double, double));


// STRUCTURE & FONCTION MUETTE UNIQUEMENT UTILISABLE POUR DOPRI45
// typedef struct
// {
//     double tn;
//     double xn;
//     double vn;
//     double an;    
// } Derive_temp;

// int methode_RK_row(const int q, Derive_temp P[q], const double A[][q], const double *B, const double *C, const double h, double t, double *x, double *v, double (*f)(double, double, double));
int methode_DOPRI45(double stepSize, double *Time, double err, double *x, double *v, double (*f)(double, double, double));


#endif //SIMULTAION_NUMERIQUE_H_INCLUED
