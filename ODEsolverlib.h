#ifndef ODESOLVERLIB_H_INCLUED
#define ODESOLVERLIB_H_INCLUED
#include <math.h>   // Used for: isnan(), pow()
#include <stddef.h> // Used for: NULL

#define ABS_ODESOLVER(x) (((x) < 0) ? -(x) : (x))
#define SQ_ODESOLVER(x) ((x)*(x))

typedef struct
{
    double x;
    double y;
} Vec2d;

typedef struct
{
    double x;
    double y;
    double z;
} Vec3d;

int ExplicitEuler(const double dt, double t, double *x, double *v, double (*f)(double, double, double));
int ExplicitEuler2d(const double dt, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d));
int ExplicitEuler3d(const double dt, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d));
int SymplecticEuler(const double dt, double t, double *x, double *v, double (*f)(double, double, double));
int SymplecticEuler2d(const double dt, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d));
int SymplecticEuler3d(const double dt, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d));

int RK4(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int RK4_2d(const double h, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d));
int RK4_3d(const double h, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d));
int RK(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int RK_2d(const double h, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d));
int RK_3d(const double h, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d));

int Verlet(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int Verlet2d(const double h, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d));
int Verlet3d(const double h, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d));


// STRUCTURE & FUNCTION MINT TO BE MUTED BECAUSE ONLY USE FOR DOPRI45
// typedef struct
// {
//     double tn;
//     double xn;
//     double vn;
//     double an;
// } dt_struct;

// int RKAdjCoef(const int q, Derive_temp P[q], const double A[][q], const double *B, const double *C, const double h, double t, double *x, double *v, double (*f)(double, double, double));

int DOPRI45(double stepSize, double Time, double err, double *x, double *v, double (*f)(double, double, double));
int DOPRI45_2d(double stepSize, double Time, double err, Vec2d *x, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d));
int DOPRI45_3d(double stepSize, double Time, double err, Vec3d *x, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d));


////////////////////////////////////////////////////////////////////////////////
// Here are the differences/derivatives apply to the space parameters of the equations

// {Stencil 3 points} (1 Dimension) continuous difference Centered
double Diff1Cent3p1DCont(double h, double x0, double (*f)(double));

// {Stencil 3 points} (1 Dimension) finite-difference Centered
double Diff1Cent3p1D(double h, double f_ip1, double f_im1);

// {Stencil 3 points} (1 Dimension) [Laplace operator] continuous difference Centered
double Diff2Cent3p1DCont(double h, double x0, double (*f)(double));

// {Stencil 3 points} (1 Dimension) [Laplace operator] finite-difference Centered
double Diff2Cent3p1D(double h, double f_ip1, double f_i, double f_im1);

// {Stencil 3 points} (2 Dimensions) [Laplace operator] finite-difference Centered
double Diff2Cent3p2D(double h, double f_ip1j, double f_ijp1, double f_ij, double f_ijm1, double f_im1j);

// {Stencil 5 points} (2 Dimensions) [Laplace operator] finite-difference Centered
double Diff2Cent5p2D(double h, double f_ip2j, double f_ip1j, double f_ijp2, double f_ijp1, double f_ij, double f_ijm1, double f_ijm2, double f_im1j, double f_im2j);

////////////////////////////////////////////////////////////////////////////////


# ifdef ODESOLVERLIB_IMPLEMENTATION


int ExplicitEuler(const double dt, double t, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
    double vv = (*v);
    if (isnan(*v)) return -1;
    // (*v) = dt*(*a) + (*v);               // v(t+1) = dt*a(t) + v(t)
    (*v) = dt*((*f)(t, *x, *v)) + (*v);     // v(t+1) = dt*a(t) + v(t)
    if (isnan(*x)) return -1;
    (*x) = (dt*(vv) + (*x));                // x(t+1) = dt*v(t) + x(t)
    return 0;
}

int ExplicitEuler2d(const double dt, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d))
{
    Vec2d vv;
    vv.x = (*v).x;
    vv.y = (*v).y;
    if (isnan(v->x) || isnan(v->y)) return -1;
    // (*v) = dt*(*a) + (*v);                 // v(t+1) = dt*a(t) + v(t)
    Vec2d tmp = (*f)(t, *p, *v);
    v->x = dt*(tmp.x) + v->x;                 // v(t+1) = dt*a(t) + v(t)
    v->y = dt*(tmp.y) + v->y;                 // v(t+1) = dt*a(t) + v(t)
    if (isnan(p->x) || isnan(p->y)) return -1;
    p->x = (dt*(vv.x) + p->x);                // x(t+1) = dt*v(t) + x(t)
    p->y = (dt*(vv.y) + p->y);                // x(t+1) = dt*v(t) + x(t)
    return 0;
}

int ExplicitEuler3d(const double dt, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d))
{
    (void)dt; (void)t; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}

int SymplecticEuler(const double dt, double t, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
    if (isnan(*v)) return -1;
    // (*v) = dt*(*a) + (*v);            // v(t+1) = dt*a(t) + v(t)
    (*v) = dt*((*f)(t, *x, *v)) + (*v);  // v(t+1) = dt*a(t) + v(t)
    if (isnan(*x)) return -1;
    (*x) = dt*(*v) + (*x);               // x(t+1) = dt*v(t+1) + x(t)
    return 0;
}

int SymplecticEuler2d(const double dt, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d))
{
    if (isnan(v->x) || isnan(v->y)) return -1;
    // (*v) = dt*(*a) + (*v);            // v(t+1) = dt*a(t) + v(t)
    Vec2d tmp = (*f)(t, *p, *v);
    v->x = dt*(tmp.x) + (v->x);  // v(t+1) = dt*a(t) + v(t)
    v->y = dt*(tmp.y) + (v->y);  // v(t+1) = dt*a(t) + v(t)
    if (isnan(p->x) || isnan(p->y)) return -1;
    p->x = dt*(v->x) + (p->x);               // x(t+1) = dt*v(t+1) + x(t)
    p->y = dt*(v->y) + (p->y);               // x(t+1) = dt*v(t+1) + x(t)
    return 0;
}

int SymplecticEuler3d(const double dt, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d))
{
    (void)dt; (void)t; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}

int RK4(const double h, double t, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
    struct
    {
    	double tn;
    	double xn;
    	double vn;
    	double an;
    } P[4] = {0};

    P[0].tn = t;
    P[0].xn = (*x);
    P[0].vn = (*v);
    P[0].an = (*f)(P[0].tn, P[0].xn, P[0].vn);

    P[1].tn = t + h*0.5f;
    P[1].xn = (*x) + h*0.5f*P[0].vn;
    P[1].vn = (*v) + h*0.5f*P[0].an;
    P[1].an = (*f)(P[1].tn, P[1].xn, P[1].vn);

    P[2].tn = t + h*0.5f;
    P[2].xn = (*x) + h*0.5f*P[1].vn;
    P[2].vn = (*v) + h*0.5f*P[1].an;
    P[2].an = (*f)(P[2].tn, P[2].xn, P[2].vn);

    P[3].tn = t + h;
    P[3].xn = (*x) + h*P[2].vn;
    P[3].vn = (*v) + h*P[2].an;
    // P[3].xn = (*x) + 0.5f*h*P[2].vn;                // Better coefficient than conventional RK4
    // P[3].vn = (*v) + 0.5f*h*P[2].an;                // Better coefficient than conventional RK4
    P[3].an = (*f)(P[3].tn, P[3].xn, P[3].vn);

    (*x) = (*x) + h*(1.f/6.f)*(P[0].vn + 2.f*P[1].vn + 2.f*P[2].vn + P[3].vn);
    if (isnan(*x)) return -1;
    (*v) = (*v) + h*(1.f/6.f)*(P[0].an + 2.f*P[1].an + 2.f*P[2].an + P[3].an);
    if (isnan(*v)) return -1;

    return 0;
}

int RK4_2d(const double h, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d))
{
    struct
    {
    	double tn;
    	Vec2d pn;
    	Vec2d vn;
    	Vec2d an;
    } P[4] = {0};

    P[0].tn = t;
    P[0].pn.x = (p->x);                    P[0].pn.y = (p->y);
    P[0].vn.x = (v->x);                    P[0].vn.y = (v->y);
    P[0].an = (*f)(P[0].tn, P[0].pn, P[0].vn);

    P[1].tn = t + h*0.5f;
    P[1].pn.x = (p->x) + h*0.5f*P[0].vn.x; P[1].pn.y = (p->y) + h*0.5f*P[0].vn.y;
    P[1].vn.x = (v->x) + h*0.5f*P[0].an.x; P[1].vn.y = (v->y) + h*0.5f*P[0].an.y;
    P[1].an = (*f)(P[1].tn, P[1].pn, P[1].vn);

    P[2].tn = t + h*0.5f;
    P[2].pn.x = (p->x) + h*0.5f*P[1].vn.x; P[2].pn.y = (p->y) + h*0.5f*P[1].vn.y;
    P[2].vn.x = (v->x) + h*0.5f*P[1].an.x; P[2].vn.y = (v->y) + h*0.5f*P[1].an.y;
    P[2].an = (*f)(P[2].tn, P[2].pn, P[2].vn);

    P[3].tn = t + h;
    P[3].pn.x = (p->x) + h*P[2].vn.x;      P[3].pn.y = (p->y) + h*P[2].vn.y;
    P[3].vn.x = (v->x) + h*P[2].an.x;      P[3].vn.y = (v->y) + h*P[2].an.y;
    // P[3].xn = (*x) + 0.5f*h*P[2].vn;                // Meilleur coeficient que RK4 classique
    // P[3].vn = (*v) + 0.5f*h*P[2].an;                // Meilleur coeficient que RK4 classique
    P[3].an = (*f)(P[3].tn, P[3].pn, P[3].vn);

    (p->x) = (p->x) + h*(1.f/6.f)*(P[0].vn.x + 2.f*P[1].vn.x + 2.f*P[2].vn.x + P[3].vn.x);
    (p->y) = (p->y) + h*(1.f/6.f)*(P[0].vn.y + 2.f*P[1].vn.y + 2.f*P[2].vn.y + P[3].vn.y);
    if (isnan(p->x) || isnan(p->y)) return -1;
    (v->x) = (v->x) + h*(1.f/6.f)*(P[0].an.x + 2.f*P[1].an.x + 2.f*P[2].an.x + P[3].an.x);
    (v->y) = (v->y) + h*(1.f/6.f)*(P[0].an.y + 2.f*P[1].an.y + 2.f*P[2].an.y + P[3].an.y);
    if (isnan(v->x) || isnan(v->y)) return -1;

    return 0;
}

int RK4_3d(const double h, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d))
{
    (void)h; (void)t; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}

int RK(const double h, double t, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
#ifndef q
# define q 4
#else
# undef q
#endif
    static const double A[q][q] = {{0  , 0, 0, 0},
    	                           {0.5f, 0, 0, 0},
    	                           {0, 0.5f, 0, 0},
    	                           {0, 0, 1.f , 0}};
    static const double B[q] = {(1.f/6.f), (1.f/3.f), (1.f/3.f), (1.f/6.f)};
    static const double C[q] = {0,
                                0.5f,
                                0.5f,
                                1.f};

    struct
    {
	double tn;
	double xn;
	double vn;
	double an;
    } P[q] = {0};

    for (int i = 0; i < q; i++) {
	double vtemp = 0;
	double atemp = 0;

	for (int k = 0; k < i+1; k++) {
	    vtemp += A[i][k]*P[k].vn;
	    atemp += A[i][k]*P[k].an;
	}
	P[i].tn = t + C[i]*h;
	P[i].xn = (*x) + h*vtemp;
	P[i].vn = (*v) + h*atemp;
	P[i].an = (*f)(P[i].tn, P[i].xn, P[i].vn); // f(P[i].tn, P[i].xn, P[i].vn) en çha
    }

    /// TODO: Redo this part to be coherent with the RK4
    double vtemp = 0;
    double atemp = 0;
    for(int i = 0; i < q; i++) {
    	vtemp += B[i]*P[i].vn;
    	atemp += B[i]*P[i].an;
    }

    if (isnan(vtemp)) return -1;
    (*x) = (*x) + h*vtemp;
    if (isnan(atemp)) return -1;
    (*v) = (*v) + h*atemp;
    ///

#ifdef q
# undef q
#endif
    return 0;
}

int RK_2d(const double h, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d))
{
#ifndef q
# define q 4
#else
# undef q
#endif
    static const double A[q][q] = {{0  , 0, 0, 0},
                                   {0.5f, 0, 0, 0},
                                   {0, 0.5f, 0, 0},
                                   {0, 0, 1.f , 0}};
    static const double B[q] = {(1.f/6.f), (1.f/3.f), (1.f/3.f), (1.f/6.f)};
    static const double C[q] = {0,
    			        0.5f,
    			        0.5f,
    			        1.f};

    struct
    {
	double tn;
	Vec2d pn;
	Vec2d vn;
	Vec2d an;
    } P[q] = {0};

    for (int i = 0; i < q; i++) {
	Vec2d vtemp = {0};
	Vec2d atemp = {0};

	for (int k = 0; k < i+1; k++) {
	    vtemp.x += A[i][k]*P[k].vn.x; vtemp.y += A[i][k]*P[k].vn.y;
	    atemp.x += A[i][k]*P[k].an.x; atemp.y += A[i][k]*P[k].an.y;
	}
	P[i].tn = t + C[i]*h;
	P[i].pn.x = (p->x) + h*vtemp.x;              P[i].pn.y = (p->y) + h*vtemp.y;
	P[i].vn.x = (v->x) + h*atemp.x;              P[i].vn.y = (v->y) + h*atemp.y;
	P[i].an = (*f)(P[i].tn, P[i].pn, P[i].vn);
    }

    /// TODO: Redo this part to be coherent with the RK4
    Vec2d vtemp = {0};
    Vec2d atemp = {0};
    for(int i = 0; i < q; i++) {
    	vtemp.x += B[i]*P[i].vn.x; vtemp.y += B[i]*P[i].vn.y;
    	atemp.x += B[i]*P[i].an.x; atemp.y += B[i]*P[i].an.y;
    }

    if (isnan(vtemp.x) || isnan(vtemp.y)) return -1;
    (p->x) = (p->x) + h*vtemp.x;
    (p->y) = (p->y) + h*vtemp.y;
    if (isnan(atemp.x) || isnan(atemp.y)) return -1;
    (v->x) = (v->x) + h*atemp.x;
    (v->y) = (v->y) + h*atemp.y;
    ///

#ifdef q
# undef q
#endif
    return 0;
}

int RK_3d(const double h, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d))
{
    (void)h; (void)t; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}

int Verlet(const double h, double t, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
    struct
    {
	double tn;
    	double xn;
    	double vn;
    	double an;
    } P = {0};

    double a = (*f)(t, *x, *v);

    P.xn = (*x) + (*v) * h + 0.5f*h*h*a;
    if (isnan(P.xn)) return -1;

    P.an = (*f)(t, P.xn, *v);
    if (isnan(P.an)) return -1;

    P.vn = (*v) + 0.5f * h*(a + P.an);
    if (isnan(P.vn)) return -1;

    (*x) = P.xn;
    (*v) = P.vn;

    return 0;
}

int Verlet2d(const double h, double t, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d))
{
    struct
    {
	double tn;
    	Vec2d pn;
    	Vec2d vn;
    	Vec2d an;
    } P = {0};

    Vec2d a = (*f)(t, *p, *v);

    P.pn.x = (p->x) + (v->y) * h + 0.5f*h*h*a.x;
    P.pn.y = (p->y) + (v->y) * h + 0.5f*h*h*a.y;
    if (isnan(P.pn.x) || isnan(P.pn.y)) return -1;

    P.an = (*f)(t, P.pn, *v);
    if (isnan(P.an.x) || isnan(P.an.y)) return -1;

    P.vn.x = (v->x) + 0.5f * h*(a.x + P.an.x);
    P.vn.y = (v->y) + 0.5f * h*(a.y + P.an.y);
    if (isnan(P.vn.x) || isnan(P.vn.y)) return -1;

    (*p) = P.pn;
    (*v) = P.vn;

    return 0;
}

int Verlet3d(const double h, double t, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d))
{
    (void)h; (void)t; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}


typedef struct
{
    double tn;
    double xn;
    double vn;
    double an;
} dt_struct;

int RKAdjCoef(const int q, dt_struct P[q], const double A[][q], const double *B, const double *C, const double h, double t, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
    for (int i = 0; i < q; i++) {
	P[i].tn = 0;
	P[i].xn = 0;
	P[i].vn = 0;
	P[i].an = 0;
    }

    for (int i = 0; i < q; i++) {
	double vtemp = 0;
	double atemp = 0;

	for (int k = 0; k < i+1; k++) {
	    vtemp += A[i][k]*P[k].vn;
	    atemp += A[i][k]*P[k].an;
	}
	P[i].tn = t + C[i]*h;
	P[i].xn = (*x) + h*vtemp;
	P[i].vn = (*v) + h*atemp;
	P[i].an = (*f)(P[i].tn, P[i].xn, P[i].vn);
    }

    double vtemp = 0;
    double atemp = 0;
    for(int i = 0; i < q; i++) {
    	vtemp += B[i]*P[i].vn;
    	atemp += B[i]*P[i].an;
    }

    if (isnan(vtemp)) return -1;
    (*x) = (*x) + h*vtemp;
    if (isnan(atemp)) return -1;
    (*v) = (*v) + h*atemp;

    return 0;
}

// Change my mind by not modifying the *Time variable.
// But let in comment the way to go back
int DOPRI45(double stepSize, double Time, double err, double *x, double *v, double (*f)(double, double, double))
{
    if ((f == NULL) || (x == NULL) || (v == NULL)) return -1;
#ifndef q
# define q 7
#else
# undef q
#endif

    static const double B4[q] = {(35.f/384.f)    , 0, (500.f/1113.f)    , (125.f/192.f)    ,-(2187.f/6784.f)     , (11.f/84.f)   , 0};
    static const double B5[q] = {(5179.f/57600.f), 0, (7571.f/16695.f)  , (393.f/640.f)    ,-(92097.f/339200.f)  , (187.f/2100.f), (1.f/40.f)};
    static const double A[q][q] = {{ 0               , 0               , 0               , 0              , 0                , 0          , 0},
    			           { (1.f/5.f)       , 0               , 0               , 0              , 0                , 0          , 0},
    			           { (3.f/40.f)      , (9.f/40.f)      , 0               , 0              , 0                , 0          , 0},
    			           { (44.f/45.f)     ,-(56.f/15.f)     , (32.f/9.f)      , 0              , 0                , 0          , 0},
    			           { (19372.f/6561.f),-(25360.f/2187.f), (64448.f/6561.f),-(212.f/729.f)  , 0                , 0          , 0},
    			           { (9017.f/3168.f) ,-(355.f/33.f)    , (46732.f/5247.f), (49.f/176.f)   , -(5103.f/18656.f), 0          , 0},
    			           { (35.f/384.f)    , 0               , (500.f/1113.f)  , (125.f/192.f)  , -(2187.f/6784.f) , (11.f/84.f), 0}};
    static const double C[q] = {0,
    			        1.f/5.f,
    			        3.f/10.f,
    			        4.f/5.f,
    			        8.f/9.f,
    			        1.f,
    			        1.f};

    dt_struct P[q] = {0};

    double dt = stepSize;
    long pas = 0;
    double bx = (*x);
    double bv = (*v);
    double TE = 0;
    double startTime = Time;
    // double startTime = (*Time);
    int firstTime = 1;
    const double ErreurDebut = err;
    double DernierDifErreur = 10e10;
    double valeur = 10e10;


    if (dt < 1e-15) {
	return -1;
    }
    do
    {
	double t = startTime;
	if (!firstTime) {
	    // (*Time) = startTime;
	    (*x) = bx;
	    (*v) = bv;
	    dt = 0.1f * (dt) * pow(err/TE, 1.f/q);
	    // could change for:
	    // dt /= 5.f;
	    if (dt < 1e-10) return -1;
	}
	pas = 0;

	while (t < (startTime + stepSize)) {
	    double h = dt;
	    if ((t + h) > (startTime + stepSize - 1e-10))
		h = startTime + stepSize - t;
	    pas++;

	    if (RKAdjCoef(q, P, A, B5, C, h, t, x, v, f) < 0) return -1;
	    // (*Time) = t;
	    t = t + h;

	    if (pas > (stepSize/dt + 2)) return -1;
	}

	double vtemp = 0;
	double atemp = 0;
    	for (int i = 0; i < q; i++) {
    	    vtemp += P[i].vn*B4[i];
    	    atemp += P[i].an*B4[i];
    	}
    	TE = 0;
    	for (int i = 0; i < q; i++) {
    	    TE += (B5[i] - B4[i])*P[i].xn;
    	}
    	TE = ABS_ODESOLVER(TE);

	const double ErreurFinale = TE;
	if (isnan(ErreurFinale)) return -1;
	const double difErreur = ABS_ODESOLVER(ErreurDebut - ErreurFinale);
	if (!firstTime)
	    valeur = ABS_ODESOLVER(difErreur - DernierDifErreur);
	DernierDifErreur = difErreur;
	firstTime = 0;
    } while(valeur > err);

#ifdef q
# undef q
#endif
    return 0;
}

int DOPRI45_2d(double stepSize, double Time, double err, Vec2d *p, Vec2d *v, Vec2d (*f)(double, Vec2d, Vec2d))
{
    (void)stepSize; (void)Time; (void)err; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}

int DOPRI45_3d(double stepSize, double Time, double err, Vec3d *p, Vec3d *v, Vec3d (*f)(double, Vec3d, Vec3d))
{
    (void)stepSize; (void)Time; (void)err; (void)p; (void)v; (void)f;
    fprintf(stdout, "WARNING: \'%s\' is not implemented yet\n", __FUNCTION__);
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Here are the differences/derivatives apply to the space parameters of the equations

double Diff1Cent3p1DCont(double h, double x0, double (*f)(double))
{
    return (f(x0 + h) - f(x0 - h))/(2.f*h);
}

double Diff1Cent3p1D(double h, double f_ip1, double f_im1)
{
    return (f_ip1 - f_im1)/(2.f*h);
}

double Diff2Cent3p1DCont(double h, double x0, double (*f)(double))
{
    return (f(x0 + h) - f(x0) + f(x0 - h))/SQ_ODESOLVER(h);
}

double Diff2Cent3p1D(double h, double f_ip1, double f_i, double f_im1)
{
    return (f_ip1 - 2.f*f_i + f_im1)/SQ_ODESOLVER(h);
}

double Diff2Cent3p2D(double h, double f_ip1j, double f_ijp1, double f_ij, double f_ijm1, double f_im1j)
{
    // (f_ip1j - 2.f*f_ij + f_im1j)/SQ_ODESOLVER(h) + (f_ijp1 - 2.f*f_ij + f_ijm1)/SQ_ODESOLVER(h);
    return (f_ip1j + f_im1j + f_ijp1 + f_ijm1 - 4.f*f_ij)/SQ_ODESOLVER(h);
}

double Diff2Cent5p2D(double h, double f_ip2j, double f_ip1j, double f_ijp2, double f_ijp1, double f_ij, double f_ijm1, double f_ijm2, double f_im1j, double f_im2j)
{
    return (-f_im2j + 16.f*f_ip1j - 30.f*f_ij + 16.f*f_im1j - f_ip2j)/(12.f*SQ_ODESOLVER(h)) + (-f_ijm2 + 16.f*f_ijp1 - 30.f*f_ij + 16.f*f_ijm1 - f_ijp2)/(12.f*SQ_ODESOLVER(h));
}
////////////////////////////////////////////////////////////////////////////////


# endif // ODESOLVERLIB_IMPLEMENTATION
#endif // ODESOLVERLIB_H_INCLUED

/***********************************
TODO:
- Suppress time the parameter in ExplicitEuler, SymplecticEuler, RK4, RK, Verlet.
- Redo this part to be coherent with the RK4 line 140
- Find an other way than using pow() in the DOPRI45
- Make sure that 'Vec2d' and 'Vec3d' will not collide with another library
***********************************/
