#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define LIBODESOLVER_IMPLEMENTATION
#include "libODEsolver.h"
#define LIBGRAPH_IMPLEMENTATION
#include "libgraph.h"
#define LIBIMAGEFILE_IMPLEMENTATION
#include "libImagefile.h"

#define FPS 60

#define HEIGHT (9*66)
#define WIDTH (16*50)

#define LENGTH_DRAG_BUFFER 500
#define FILEPATH "test.csv"
#define POW2(x) ((x)*(x))
#define UNUSED(x) (void)(x)
#ifndef ABS
# define ABS(x) ((x < 0) ? -(x) : (x))
#endif


double equ_psi_1(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return ((-g*(2.f*m1 + m2)*sin(th1)) - m2*g*sin(th1 - 2.f*th2) - 2.f*sin(th1 - th2)*m2*(ph2*ph2*l2 + ph1*ph1*l1*cos(th1 - th2)))/(l1*(2*m1 + m2 - m2*cos(2.f*th1 - 2.f*th2)));
}

double equ_psi_2(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (2.f*sin(th1 - th2)*(ph1*ph1*l1*(m1 + m2) + g*(m1 + m2)*cos(th1) + ph2*ph2*l2*m2*cos(th1 - th2)))/(l2*(2.f*m1 + m2 - m2*cos(2.f*th1 - 2.f*th2)));
}


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
    double equ_var1_psi_1(double x, double th1, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_1(th1, Var_Dp1.theta_2, Var_Dp1.phi_1, Var_Dp1.phi_2, Var_Dp1.g, Var_Dp1.l1, Var_Dp1.l2, Var_Dp1.m1, Var_Dp1.m2); \
    }									\
    double equ_var1_psi_2(double x, double th2, double y)		\
    {									\
	UNUSED(x);							\
	UNUSED(y);							\
	return equ_psi_2(Var_Dp1.theta_1, th2, Var_Dp1.phi_1, Var_Dp1.phi_2, Var_Dp1.g, Var_Dp1.l1, Var_Dp1.l2, Var_Dp1.m1, Var_Dp1.m2); \
    }									\
    do{}while(0)							\

typedef struct
{
    int x;
    int y;
} Vector2;
	

typedef struct
{
    Vector2 mass1;
    Vector2 mass2;
    Vector2 bufDrag[LENGTH_DRAG_BUFFER];
} Double_pendulum;


void state_save(Var_Dp Dp, Var_Dp *buf)
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

void state_restore(Var_Dp *Dp, Var_Dp buf)
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


int methode_RK_adaptative_pendulum(double stepSize, double err, Var_Dp *Dp, double (*f)(double, double, double), double (*g)(double, double, double), int (*ODESolver)(const double, double, double *, double *, double (*)(double, double, double)))
{
    // Saving the ODE state
    double dt = stepSize;
    long steps = 0;
    Var_Dp DpB = {0};
    state_save(*Dp, &DpB); // Saving to be able to modify a copy as desired

    double startTime = DpB.t;
    const double startEnergy = DpB.e;
    
    double lastEnergyDif = 10e10;
    double valeur = 10e10;
    int firstTime = 1;

    if (stepSize < 1e-15)
	return -1;
    do
    {
	double t = startTime;
	if (!firstTime) {
	    state_restore(Dp, DpB); // Restore the Dp variable as before
	    dt /= 5;
	    if (dt < 1e-10) {
		fprintf(stderr, "WARNING: Time steps too small\n");
		return -1;
	    }
	}
	steps = 0;

	while (t < (startTime + stepSize)) {
	    double h = dt;

	    if ((t + h) > (startTime + stepSize - 1e-10))
		h = startTime + stepSize - t;
	    steps++;
	    if (ODESolver(h, t, &Dp->theta_1, &Dp->phi_1, f) < 0) return -1; // New step
	    if (ODESolver(h, t, &Dp->theta_2, &Dp->phi_2, g) < 0) return -1; // New step
	    Dp->t = t; // Save all the data from the Dp variable
	    t = t + h;
	    if (steps > (stepSize/dt + 2)){ 
	    	fprintf(stderr, "WARNING: Number of steps too big: %ld, %f\n", steps, stepSize/dt);
		return -1;
	    }
	}

	const double endEnergy = get_pendulum_energy(*Dp);
	if (isnan(endEnergy)) return -1;
	
	const double EnergyDif = ABS(startEnergy - endEnergy);
	if (!firstTime) valeur = ABS(EnergyDif - lastEnergyDif);
	
	lastEnergyDif = EnergyDif;
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

void Draw_double_pendulum(int i, Double_pendulum *Dp, Var_Dp VDp, uint8_t ***cl)
{
    draw_clear(cl, WIDTH, HEIGHT);

    // Calculations of the new positions in Cartesian coordinates
    Dp->mass1.x = 100.f*VDp.l1*sin(VDp.theta_1);
    Dp->mass1.y = -100.f*VDp.l1*cos(VDp.theta_1);
    Dp->mass2.x = 100.f*VDp.l2*sin(VDp.theta_2) + Dp->mass1.x;
    Dp->mass2.y = -100.f*VDp.l2*cos(VDp.theta_2) + Dp->mass1.y;

    // Ignore things outside window
    Dp->mass1 = overflow_protection_window(Dp->mass1);
    Dp->mass2 = overflow_protection_window(Dp->mass2);

    // First, draw the pendulum rods
    draw_ligne(cl, WIDTH,  HEIGHT, 0, 0, Dp->mass1.x, -Dp->mass1.y, 0xFFFFFF00);
    draw_ligne(cl, WIDTH,  HEIGHT, Dp->mass1.x, -Dp->mass1.y, Dp->mass2.x, -Dp->mass2.y, 0xFFFFFF00);
    
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
    	draw_ligne(cl, WIDTH, HEIGHT, Dp->bufDrag[j].x, -Dp->bufDrag[j].y, Dp->bufDrag[j+1].x, -Dp->bufDrag[j+1].y, 0x00ff00000);
    }
    
    // Third, draw the pendulum's masses
    draw_cercle(cl, WIDTH, HEIGHT, Dp->mass1.x, -Dp->mass1.y, 5.f*VDp.m1, 0xFF000000);
    draw_cercle(cl, WIDTH, HEIGHT, Dp->mass2.x, -Dp->mass2.y, 5.f*VDp.m2, 0xFF000000);
}



int main()
{    
    Var_Dp Var_Dp1 = {.l1       = 1.0f,
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
    Var_Dp1.e = get_pendulum_energy(Var_Dp1);

    VARIABLE_PENDULUM_INIT;

    double dt = 2.f/FPS;
    double epsilon = 0.001f;
    // float t = 0;
    
    Double_pendulum Dp1;

    uint8_t ***color = b24_color_alloc(WIDTH, HEIGHT);
    char filepath_image[FPS*10][40];

    for (int i = 0; i < FPS*10; i++)
	sprintf(filepath_image[i], "stock/Double_pendulum_%03d.ppm", i);

    for (int i = 0; i < FPS*10; i++) {

	if (methode_RK_adaptative_pendulum(dt, epsilon, &Var_Dp1, equ_var1_psi_1, equ_var1_psi_2, methode_RK4) < 0)
	    fprintf(stderr, "ERROR: Calculation overflow from: %s\n", "methode_RK_adaptative_pendulum");

	Draw_double_pendulum(i, &Dp1, Var_Dp1, color);

	CreateImagePPM24b(filepath_image[i], color, WIDTH, HEIGHT);
    }
    b24_color_free(color, WIDTH, HEIGHT);
    return 0;
}
