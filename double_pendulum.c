#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define LIBODESOLVER_IMPLEMENTATION
#include "libODEsolver.h"

#define HEIGHT 600
#define WIDTH 800
#define FPS 60

#define LENGTH_DRAG_BUFFER 500
#define POW2(x) ((x)*(x))
#define UNUSED(x) (void)(x)
#ifndef ABS
# define ABS(x) ((x < 0) ? -(x) : (x))
#endif
#define TEST_FILE(x, f)				\
do {						\
    if (x) {					\
	fprintf(stderr, "ERROR: Failed to write file: %s\n", f);	\
	exit(1);				\
    }						\
} while (0)					\


double equ_psi_1(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return ((-g*(2.f*m1 + m2)*sin(th1)) - m2*g*sin(th1 - 2.f*th2) - 2.f*sin(th1 - th2)*m2*(ph2*ph2*l2 + ph1*ph1*l1*cos(th1 - th2)))/(l1*(2*m1 + m2 - m2*cos(2.f*th1 - 2.f*th2)));
}

double equ_psi_2(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (2.f*sin(th1 - th2)*(ph1*ph1*l1*(m1 + m2) + g*(m1 + m2)*cos(th1) + ph2*ph2*l2*m2*cos(th1 - th2)))/(l2*(2.f*m1 + m2 - m2*cos(2.f*th1 - 2.f*th2)));
}

/// EQUATIONS MADE WITHOUT SIMPLIFICATIONS ///////
double equ_psi_1_main(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (l2*m2*(l2*m2*ph2*ph2*sin(th1 - th2) + (m1 + m2)*g*sin(th1)) + l2*m2*cos(th1 - th2)*(l1*m2*ph1*ph1*sin(th1 - th2) - m2*g*sin(th2))) / (l1*l2*m2*m2*cos(th1 - th2)*cos(th1 - th2) - l1*l2*m2*(m1 + m2));
}
double equ_psi_2_main(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (-l1*(m1 + m2)*(l1*m2*ph1*ph1*sin(th1 - th2) - m2*g*sin(th2)) - l1*m2*cos(th1 - th2)*(l2*m2*ph2*ph2*sin(th1 - th2) + (m1 + m2)*g*sin(th1))) / (l1*l2*m2*m2*cos(th1 - th2)*cos(th1 - th2) - l1*l2*m2*(m1 + m2));
}
//////////////////////////////////////////////////

typedef struct
{
    const float  l1;
    const float  l2;
    const float  m1;
    const float  m2;

    const float  g;

    double t;
    double e;
    double theta_1;
    double phi_1;
    double psi_1;

    double theta_2;
    double phi_2;
    double psi_2;
} Var_Dp;


#define VARIABLE_PENDULUM_INIT						\
    double equ_psi_1_var1(double x, double th1, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_1(th1, Var_Dp1.theta_2, Var_Dp1.phi_1, Var_Dp1.phi_2, Var_Dp1.g, Var_Dp1.l1, Var_Dp1.l2, Var_Dp1.m1, Var_Dp1.m2); \
    }									\
    double equ_psi_2_var1(double x, double th2, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_2(Var_Dp1.theta_1, th2, Var_Dp1.phi_1, Var_Dp1.phi_2, Var_Dp1.g, Var_Dp1.l1, Var_Dp1.l2, Var_Dp1.m1, Var_Dp1.m2); \
    }									\
    double equ_psi_1_var2(double x, double th1, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_1(th1, Var_Dp2.theta_2, Var_Dp2.phi_1, Var_Dp2.phi_2, Var_Dp2.g, Var_Dp2.l1, Var_Dp2.l2, Var_Dp2.m1, Var_Dp2.m2); \
    }									\
    double equ_psi_2_var2(double x, double th2, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_2(Var_Dp2.theta_1, th2, Var_Dp2.phi_1, Var_Dp2.phi_2, Var_Dp2.g, Var_Dp2.l1, Var_Dp2.l2, Var_Dp2.m1, Var_Dp2.m2); \
    }									\
    double equ_psi_1_var2_main(double x, double th1, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_1_main(th1, Var_Dp2.theta_2, Var_Dp2.phi_1, Var_Dp2.phi_2, Var_Dp2.g, Var_Dp2.l1, Var_Dp2.l2, Var_Dp2.m1, Var_Dp2.m2); \
    }									\
    double equ_psi_2_var2_main(double x, double th2, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_1_main(Var_Dp2.theta_1, th2, Var_Dp2.phi_1, Var_Dp2.phi_2, Var_Dp2.g, Var_Dp2.l1, Var_Dp2.l2, Var_Dp2.m1, Var_Dp2.m2); \
    }									\
    do{}while(0)							\

typedef struct
{
    Vector2 mass1;
    Vector2 mass2;
    Vector2 bufDrag[LENGTH_DRAG_BUFFER];
} double_pendulum;


void save_state(Var_Dp Dp, Var_Dp *buf)
{    
    buf->e = Dp.e;
    buf->t = Dp.t;
    buf->theta_1 = Dp.theta_1;
    buf->theta_2 = Dp.theta_2;
    buf->phi_1 = Dp.phi_1;
    buf->phi_2 = Dp.phi_2;
    buf->psi_1 = Dp.psi_1;
    buf->psi_2 = Dp.psi_2;
}

void restaure_state(Var_Dp *Dp, Var_Dp buf)
{    
    Dp->e = buf.e;
    Dp->t = buf.t;
    Dp->theta_1 = buf.theta_1;
    Dp->theta_2 = buf.theta_2;
    Dp->phi_1 = buf.phi_1;
    Dp->phi_2 = buf.phi_2;
    Dp->psi_1 = buf.psi_1;
    Dp->psi_2 = buf.psi_2;
}

double get_pendulum_energy(Var_Dp Dp)
{
    float T_v2 = (1.f/2.f)*(Dp.m1 + Dp.m2)*POW2(Dp.l1)*POW2(Dp.phi_1) + (1.f/2.f)*Dp.m2*POW2(Dp.l2)*POW2(Dp.phi_2) + Dp.m2*Dp.l1*Dp.l2*Dp.phi_1*Dp.phi_2*cos(Dp.theta_1 - Dp.theta_2);
    float V_v2 = -(Dp.m1 + Dp.m2)*Dp.g*Dp.l1*cos(Dp.theta_1) - Dp.m2*Dp.g*Dp.l2*cos(Dp.theta_2);
    return ABS(T_v2 + V_v2);
}


int adaptive_method(double stepSize, double err, Var_Dp *Dp, double (*f)(double, double, double), double (*g)(double, double, double), int (*ODESolver)(const double, double, double *, double *, double (*)(double, double, double)))
{
    double dt = stepSize;
    long pas = 0;
    Var_Dp DpB = {0};
    save_state(*Dp, &DpB); // Save *Dp into buffer to be able to edit Dp as desired

    double startTime = DpB.t;
    const double EnergieDebut = DpB.e;    
    double DernierDifEnergie = 10e10;
    double valeur = 10e10;
    int firstTime = 1;

    if (stepSize < 1e-15)
	return -1;
    do
    {
	double t = startTime;
	if (!firstTime) {
	    restaure_state(Dp, DpB); // restore Dp as before
	    // Save all the data of Dp
	    dt /= 5;
	    if (dt < 1e-10) {
		fprintf(stderr, "ERROR: Step time too small < 1e-10\n");
		return -1;
	    }
	}
	pas = 0;

	while (t < (startTime + stepSize)) {
	    double h = dt;

	    if ((t + h) > (startTime + stepSize - 1e-10))
		h = startTime + stepSize - t;
	    pas++;

	    if (ODESolver(h, t, &Dp->theta_1, &Dp->phi_1, f) < 0) return -1; // New step
	    if (ODESolver(h, t, &Dp->theta_2, &Dp->phi_2, g) < 0) return -1; // New step

	    Dp->t = t; // Save all the data of Dp

	    t = t + h;
	    if (pas > (stepSize/dt + 2)){ 
	    	fprintf(stderr, "ERROR: too much steps: %ld, %f\n", pas, stepSize/dt);
		return -1;
	    }
	}

	const double EnergieFinale = get_pendulum_energy(*Dp);
	if (isnan(EnergieFinale)) return -1;
	const double difEnergie = ABS(EnergieDebut - EnergieFinale);
	if (!firstTime)
	    valeur = ABS(difEnergie - DernierDifEnergie);

	DernierDifEnergie = difEnergie;
	firstTime = 0;
    } while (valeur > err);
    return 0;
}




Vector2 overflow_protection_window(Vector2 a)
{
    if ((a.x + WIDTH/2) < 1)
	a.x = -WIDTH/2 + 1;
    if ((a.x + WIDTH/2) > WIDTH-1)
	a.x = WIDTH/2 - 1;
    if ((a.y + HEIGHT/2) < 1)
	a.y = -HEIGHT/2 + 1;
    if ((a.y + HEIGHT/2) > HEIGHT-1)
	a.y = HEIGHT/2 - 1;
    return a;
}

void Draw_double_pendulum(int i, double_pendulum *Dp, Color cl, Var_Dp VDp)
{
    // Calculations of the new positions in Cartesian coordinates
    Dp->mass1.x = 100.f*VDp.l1*sin(VDp.theta_1);
    Dp->mass1.y = -100.f*VDp.l1*cos(VDp.theta_1);
    Dp->mass2.x = 100.f*VDp.l2*sin(VDp.theta_2) + Dp->mass1.x;
    Dp->mass2.y = -100.f*VDp.l2*cos(VDp.theta_2) + Dp->mass1.y;

    // Ignore things outside window
    Dp->mass1 = overflow_protection_window(Dp->mass1);
    Dp->mass2 = overflow_protection_window(Dp->mass2);
    
    // First, draw the pendulum rods
    DrawLine(WIDTH/2, HEIGHT/2, Dp->mass1.x + WIDTH/2, HEIGHT/2 - Dp->mass1.y, WHITE);
    DrawLine(Dp->mass1.x + WIDTH/2, HEIGHT/2 - Dp->mass1.y, Dp->mass2.x + WIDTH/2, HEIGHT/2 - Dp->mass2.y, WHITE);

    // Set the new position of the second mass in the beginning of the buffer
    Dp->bufDrag[0].x = Dp->mass2.x;
    Dp->bufDrag[0].y = Dp->mass2.y;

    // Update the new trajectory's drag into the buffer
    for (int j = i-1; j > 0; j--) {
    	Dp->bufDrag[j].x = Dp->bufDrag[j-1].x;
    	Dp->bufDrag[j].y = Dp->bufDrag[j-1].y;
    }
	
    // Second, draw the trajectory's drag of the second mass of pendulum
    for (int j = 0; j < i-1; j++){
	DrawLine(Dp->bufDrag[j].x + WIDTH/2, HEIGHT/2 - Dp->bufDrag[j].y, Dp->bufDrag[j+1].x + WIDTH/2, HEIGHT/2 - Dp->bufDrag[j+1].y, cl);
    }
    
    // Third, draw the pendulum's masses
    DrawCircle(Dp->mass1.x + WIDTH/2, HEIGHT/2 - Dp->mass1.y, 5.f*VDp.m1, RED);
    DrawCircle(Dp->mass2.x + WIDTH/2, HEIGHT/2 - Dp->mass2.y, 5.f*VDp.m2, RED);
}


int main(int argc, char *argv[])
{
    argc--;argv++;
    int save = 0;
    int res = -1;
    FILE *file = NULL;
    
    if (argc == 1) res = atoi(argv[0]);
    switch (res) {
        case 0:
	    fprintf(stderr, "WARNING: Wrong argument = NO SAVES\n");
	    break;
	case 1: // save file energy
	    save = 1;
	    fprintf(stderr, "INFO: Save the total energy of the double pendulum\n");
	    break;
	case 2: // save file position
	    save = 2;
	    fprintf(stderr, "INFO: Save the position of the double pendulum\n");
	    break;
	default:
	    fprintf(stderr, "INFO: No saves\n");
	    break;
    }

    if (save == 1) {
	file = fopen("Energy_backup.csv", "w");
	TEST_FILE(file == NULL, "Energy_backup.csv");
    }
    else if (save == 2) {
	file = fopen("Position_backup.csv", "w");
	TEST_FILE(file == NULL, "Position_backup.csv");
    }

    Var_Dp Var_Dp1 = {.l1       = 1.75f,
    		      .l2       = 0.5f,
    		      .m1       = 2.f,
    		      .m2       = 5.f,
    		      .g        = 9.8f,
		      .t        = 0,
    		      .theta_1  = M_PI-0.1f,
    		      .phi_1    = 0,
    		      .psi_1    = 0,
    		      .theta_2  = M_PI,
    		      .phi_2    = 0,
    		      .psi_2    = 0,
    };
    Var_Dp1.e = get_pendulum_energy(Var_Dp1);

    Var_Dp Var_Dp2 = {.l1       = 1.0f,
    		      .l2       = 1.0f,
    		      .m1       = 1.f,
    		      .m2       = 1.f,
    		      .g        = 9.8f,
		      .t        = 0,
    		      .theta_1  = M_PI-0.1f,
    		      .phi_1    = 0,
    		      .psi_1    = 0,
    		      .theta_2  = M_PI,
    		      .phi_2    = 0,
    		      .psi_2    = 0,
    };
    Var_Dp2.e = get_pendulum_energy(Var_Dp2);
    VARIABLE_PENDULUM_INIT;

    double dt = 1.f/(FPS);

    double epsilon1 = 0.01f;
    // double epsilon2 = 0.0000000001f;
    double epsilon2 = 0.001f;
    double t1 = 0;
    
    double_pendulum Dp1;
    double_pendulum Dp2;

    InitWindow(WIDTH, HEIGHT, "Double Pendulum Simulation");
    SetTargetFPS(FPS);

    char tab[30] = {' '};
    snprintf(tab, 30, "ERROR: Calculation overflow");
    for (int i = 0;!WindowShouldClose();) {
	if (IsKeyDown(KEY_SPACE)) {BeginDrawing(); EndDrawing(); continue;}
	BeginDrawing();
	ClearBackground(BLACK);

	// Adaptive method used by myphysicslab
	if (adaptive_method(dt, epsilon1, &Var_Dp1, equ_psi_1_var1, equ_psi_2_var1, methode_RK4) < 0)
	    DrawText(tab, WIDTH/2-200, 10, 50, RED);

	// Classic explicit method to compute these 2 equations
	if (methode_DOPRI45(dt, t1, epsilon2, &Var_Dp2.theta_1, &Var_Dp2.phi_1, equ_psi_1_var2) < 0)
	    DrawText(tab, WIDTH/2-200, 10, 50, RED);
	if (methode_DOPRI45(dt, t1, epsilon2, &Var_Dp2.theta_2, &Var_Dp2.phi_2, equ_psi_2_var2) < 0)
	    DrawText(tab, WIDTH/2-200, 10, 50, RED);

	if (save == 1) {
	    float E_TOT_1 = get_pendulum_energy(Var_Dp1);
	    float E_TOT_2 = get_pendulum_energy(Var_Dp2);
	    fprintf(file, "%lf,%.3f,%.3f\n", t1, E_TOT_1, E_TOT_2);
	}
	else if (save == 2) {
            fprintf(file, "%.3f,%.3f\n", sin(Var_Dp1.theta_2) + sin(Var_Dp1.theta_1), -cos(Var_Dp1.theta_2) - cos(Var_Dp1.theta_1));
	}
	
	if (i < LENGTH_DRAG_BUFFER) i++;
	
	Draw_double_pendulum(i, &Dp1, BLUE, Var_Dp1);
	Draw_double_pendulum(i, &Dp2, GREEN, Var_Dp2);
	
	snprintf(tab, 15, "t = %.2lf", t1);
	t1 += dt;

	DrawText(tab, 20, 40, 20, GREEN);
	DrawFPS(20, 20);
	EndDrawing();
    }
    if (save > 0) fclose(file);

    CloseWindow();
    return 0;
}
