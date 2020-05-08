#include <allegro5/allegro5.h>
#include "collision.h"
#define DEG_TO_RAD(x) ((float) (x)*ALLEGRO_PI/180)
#define FLIP_COIN(x, y) (( rand() % 2) ? (x) : (y))
#define RAND_DOUBLE_RANGE(min, max) ((float)rand() / (float) (RAND_MAX / (max - min)) + min)
//This will be moved to somewhere else later.
#define SCREEN_WIDTH 1080 
#define SCREEN_HEIGHT 640

#define MAX_ASTEROID_COUNT 20  //Max allowed asteroids on screen.
#define SPAWN_RATE 90   //Asteroids will spawn slowly if the spawn rate is high and vice-versa.

typedef struct{
    float x, y, heading, twist, speed, rot_velocity, scale;
    bool gone;
    ALLEGRO_COLOR color;
    BOUNDING_CIRCLE circle;
}ASTEROID;

extern ALLEGRO_TRANSFORM asteroid_transform;
extern int asteroid_count;
extern ASTEROID asteroids[MAX_ASTEROID_COUNT];
void init_asteroids();
static void create_asteroid(ASTEROID* asteroid);
static void create_new_asteroid();
void update_asteroids();
void draw_all_asteroids();
static void draw_asteroid(ASTEROID *a);
void asteroid_trigger();
