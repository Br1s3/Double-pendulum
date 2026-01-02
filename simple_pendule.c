#include <raylib.h>
#include <math.h>
// #include <stdio.h>
#include <time.h>

#define HEIGHT 800
#define WIDTH 1000
#define IPS 60

#define BUFFER_LENGTH 30

#define FACTOR 100.f

int main()
{
    // srand(time(NULL));
    Vector2 coord_boule_1[2] = {0};
    Vector2 buf_traine[BUFFER_LENGTH] = {0};

    coord_boule_1[0].x = WIDTH/2;
    coord_boule_1[0].y = HEIGHT/2;
    
    coord_boule_1[1].x = coord_boule_1[0].x;
    coord_boule_1[1].y = coord_boule_1[0].y;

    InitWindow(WIDTH, HEIGHT, "Test double pendule");
    SetTargetFPS(IPS);


    const float g = 9.8f;
    const float l = 1.f;
    const float dt = 1.f/(1.5f*IPS);
    float phi = 0;
    float theta = M_PI/2.f;

    for (int i = 0;!WindowShouldClose();) {
	BeginDrawing();
	ClearBackground(BLACK);

	theta = theta + dt * phi;
	phi = phi - dt * (g/l) * sinf(theta);
	
	coord_boule_1[0].x = FACTOR*l*cosf(theta+M_PI/2);
	coord_boule_1[0].y = FACTOR*l*sinf(theta+M_PI/2);
	
	if (!(((coord_boule_1[0].x + WIDTH/2) < 10 || (coord_boule_1[0].x + WIDTH/2) > WIDTH-10) || ((coord_boule_1[0].y + HEIGHT/2) < 10 || (coord_boule_1[0].y + HEIGHT/2) > HEIGHT-10))) {
	    coord_boule_1[1].x = coord_boule_1[0].x;
	    coord_boule_1[1].y = coord_boule_1[0].y;
	}

	if (i < BUFFER_LENGTH)
	    i++;
	buf_traine[0].x = coord_boule_1[1].x;
	buf_traine[0].y = coord_boule_1[1].y;

	for (int j = i; j > 0; j--) {
	    buf_traine[j].x = buf_traine[j-1].x;
	    buf_traine[j].y = buf_traine[j-1].y;
	}

	// trainé de la trajectoire
	for (int j = 0; j < i; j++)
	    DrawLine(buf_traine[j].x + WIDTH/2, buf_traine[j].y + HEIGHT/2, buf_traine[j+1].x + WIDTH/2, buf_traine[j+1].y + HEIGHT/2, GREEN);
	// Ligne entre le centre et le bout du pendule
	DrawLine(WIDTH/2, HEIGHT/2, coord_boule_1[1].x + WIDTH/2, coord_boule_1[1].y + HEIGHT/2, WHITE);
	// Tracé de la boule
	DrawCircle(coord_boule_1[1].x + WIDTH/2, coord_boule_1[1].y + HEIGHT/2, 5.f, RED);
	DrawFPS(20, 20);
	EndDrawing();	
    }
    CloseWindow();
    return 0;
}
