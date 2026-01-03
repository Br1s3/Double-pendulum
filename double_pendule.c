#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "resolveur_EDO.h"

#define HEIGHT 600
#define WIDTH 800
#define IPS 600

#define BUFFER_LENGTH_TRAINE 500
#define FILEPATH "test.csv"
#define carre(x) ((x)*(x))
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

/// EQUATIONS FAITES SANS SIMPLIFICATION
double equ_psi_1_main(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (l2*m2*(l2*m2*ph2*ph2*sin(th1 - th2) + (m1 + m2)*g*sin(th1)) + l2*m2*cos(th1 - th2)*(l1*m2*ph1*ph1*sin(th1 - th2) - m2*g*sin(th2))) / (l1*l2*m2*m2*cos(th1 - th2)*cos(th1 - th2) - l1*l2*m2*(m1 + m2));
}
double equ_psi_2_main(double th1, double th2, double ph1, double ph2, double g, double l1, double l2, double m1, double m2)
{
    return (-l1*(m1 + m2)*(l1*m2*ph1*ph1*sin(th1 - th2) - m2*g*sin(th2)) - l1*m2*cos(th1 - th2)*(l2*m2*ph2*ph2*sin(th1 - th2) + (m1 + m2)*g*sin(th1))) / (l1*l2*m2*m2*cos(th1 - th2)*cos(th1 - th2) - l1*l2*m2*(m1 + m2));
}
///

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
    Vector2 boule_1;
    Vector2 boule_2;
    Vector2 buf_traine[BUFFER_LENGTH_TRAINE];
} Double_pendule;


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

double get_energie_pendule(Var_Dp Dp)
{
    float T_v2 = (1.f/2.f)*(Dp.m1 + Dp.m2)*carre(Dp.l1)*carre(Dp.phi_1) + (1.f/2.f)*Dp.m2*carre(Dp.l2)*carre(Dp.phi_2) + Dp.m2*Dp.l1*Dp.l2*Dp.phi_1*Dp.phi_2*cos(Dp.theta_1 - Dp.theta_2);
    float V_v2 = -(Dp.m1 + Dp.m2)*Dp.g*Dp.l1*cos(Dp.theta_1) - Dp.m2*Dp.g*Dp.l2*cos(Dp.theta_2);
    return ABS(T_v2 + V_v2);
}


int methode_RK_adaptative_pendule(double stepSize, double err, Var_Dp *Dp, double (*f)(double, double, double), double (*g)(double, double, double), int (*ODESolver)(const double, double, double *, double *, double (*)(double, double, double)))
{
    double dt = stepSize;
    long pas = 0;
    Var_Dp DpB = {0};
    save_state(*Dp, &DpB); // Sauvegarder *Dp dans le buffer pour modifier Dp comme on le souhaite

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
	    restaure_state(Dp, DpB); // restaurer le Dp comme avant
	    // sauvegarde de toutes les données du Dp
	    dt /= 5;
	    if (dt < 1e-10) {
		fprintf(stderr, "pas de temps trop petit\n");
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

	    Dp->t = t; // sauvegarde de toutes les données du Dp

	    t = t + h;
	    if (pas > (stepSize/dt + 2)){ 
	    	fprintf(stderr, "Problème nombre de pas trop grand: %ld, %f\n", pas, stepSize/dt);
		return -1;
	    }
	}

	const double EnergieFinale = get_energie_pendule(*Dp);
	if (isnan(EnergieFinale)) return -1;
	const double difEnergie = ABS(EnergieDebut - EnergieFinale);
	if (!firstTime)
	    valeur = ABS(difEnergie - DernierDifEnergie);
	// test code
	DernierDifEnergie = difEnergie;
	firstTime = 0;
    } while (valeur > err);
    return 0;
}




Vector2 adaptation_coord_fenetre(Vector2 a)
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

void tracage_double_pendule(int i, Double_pendule *Dp, Color cl, Var_Dp VDp)
{
    Dp->boule_1.x = 100.f*VDp.l1*sin(VDp.theta_1);
    Dp->boule_1.y = -100.f*VDp.l1*cos(VDp.theta_1);
    Dp->boule_2.x = 100.f*VDp.l2*sin(VDp.theta_2) + Dp->boule_1.x;
    Dp->boule_2.y = -100.f*VDp.l2*cos(VDp.theta_2) + Dp->boule_1.y;


    Dp->boule_1 = adaptation_coord_fenetre(Dp->boule_1);
    Dp->boule_2 = adaptation_coord_fenetre(Dp->boule_2);
    
    // Ligne entre le centre et le bout du pendule
    DrawLine(WIDTH/2, HEIGHT/2, Dp->boule_1.x + WIDTH/2, HEIGHT/2 - Dp->boule_1.y, WHITE);
    DrawLine(Dp->boule_1.x + WIDTH/2, HEIGHT/2 - Dp->boule_1.y, Dp->boule_2.x + WIDTH/2, HEIGHT/2 - Dp->boule_2.y, WHITE);
    // Tracé de la boule
    DrawCircle(Dp->boule_1.x + WIDTH/2, HEIGHT/2 - Dp->boule_1.y, 5.f*VDp.m1, RED);
    DrawCircle(Dp->boule_2.x + WIDTH/2, HEIGHT/2 - Dp->boule_2.y, 5.f*VDp.m2, RED);


    Dp->buf_traine[0].x = Dp->boule_2.x;
    Dp->buf_traine[0].y = Dp->boule_2.y;

    for (int j = i-1; j > 0; j--) {
    	Dp->buf_traine[j].x = Dp->buf_traine[j-1].x;
    	Dp->buf_traine[j].y = Dp->buf_traine[j-1].y;
    }
	
    // trainé de la trajectoire
    for (int j = 0; j < i-1; j++){
	DrawLine(Dp->buf_traine[j].x + WIDTH/2, HEIGHT/2 - Dp->buf_traine[j].y, Dp->buf_traine[j+1].x + WIDTH/2, HEIGHT/2 - Dp->buf_traine[j+1].y, cl);
    }
}

int main()
{
#ifdef SAUVEGARDE_ENERGIE
    FILE *fichier = fichier = fopen(FILEPATH, "w");
    if(!fichier) fprintf(stderr, "Erreur d'écriture sur fichier\n");
#endif
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
    Var_Dp1.e = get_energie_pendule(Var_Dp1);

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
    Var_Dp2.e = get_energie_pendule(Var_Dp2);
    VARIABLE_PENDULUM_INIT;

    double dt1 = 1.f/(IPS);
    double dt2 = 1.f/(IPS);

    double epsilon1 = 0.01f;
    double epsilon2 = 0.01f;
    double t = 0;
    
    Double_pendule Dp1;
    Double_pendule Dp2;

    InitWindow(WIDTH, HEIGHT, "Test double pendule");
    SetTargetFPS(IPS);

    for (int i = 0;!WindowShouldClose();) {
	char tab[30] = {' '};
	snprintf(tab, 15, "ERREUR CALCULE");

	BeginDrawing();
	ClearBackground(BLACK);

	// Méthode adaptative utilisé sur myphysicslab
	if (methode_RK_adaptative_pendule(dt1, epsilon1, &Var_Dp1, equ_psi_1_var1, equ_psi_2_var1, methode_RK4) < 0)
	    DrawText(tab, WIDTH/2-200, 10, 50, RED);

	// Méthode explicite classique de calcul des 2 équations
	if (methode_DOPRI45(dt2, &t, epsilon2, &Var_Dp2.theta_1, &Var_Dp2.phi_1, equ_psi_1_var2) < 0)
	    DrawText(tab, WIDTH/2-200, 10, 50, RED);
	if (methode_DOPRI45(dt2, &t, epsilon2, &Var_Dp2.theta_2, &Var_Dp2.phi_2, equ_psi_2_var2) < 0)
	    DrawText(tab, WIDTH/2-200, 10, 50, RED);

#ifdef SAUVEGARDE_ENERGIE
	float E_TOT_1 = get_energie_pendule(Var_Dp1);
	float E_TOT_2 = get_energie_pendule(Var_Dp2);
	fprintf(fichier, "%lf,%.3f,%.3f\n", t, E_TOT_1, E_TOT_2);
#endif
	if (i < BUFFER_LENGTH_TRAINE) i++;
	
	tracage_double_pendule(i, &Dp1, BLUE, Var_Dp1);
	tracage_double_pendule(i, &Dp2, GREEN, Var_Dp2);
	
	snprintf(tab, 15, "t = %.2lf", t);
	t += dt1;
	DrawText(tab, 20, 40, 20, GREEN);
	DrawFPS(20, 20);
	EndDrawing();
    }
#ifdef SAUVEGARDE_ENERGIE
    fclose(fichier);
#endif
    CloseWindow();
    return 0;
}
