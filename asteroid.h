#ifndef FILE_ASTEROID_SEEN
#define FILE_ASTEROID_SEEN 1
#include <allegro5/allegro5.h>
#include "collision.h"
#define MAX_ASTEROID_COUNT 20  //Max allowed asteroids on screen.
#define SPAWN_RATE 50   //Asteroids will spawn slowly if the spawn rate is high and vice-versa.

typedef struct{
    float x, y, heading, twist, speed, rot_velocity, scale;
    bool gone;
    int life;
    ALLEGRO_COLOR color;
    enum asteroid_type{ ROCKY, STONEY}type;
    //type is used to determine which bitmap to use for drawing the asteroid.
    BOUNDING_CIRCLE circle;
}ASTEROID;

extern ALLEGRO_TRANSFORM asteroid_transform;
extern int asteroid_count;
extern ASTEROID asteroids[MAX_ASTEROID_COUNT];

void init_asteroids();
void destroy_asteroids();
void update_asteroids();
void draw_all_asteroids();
void asteroid_trigger();

#endif //asteroid.h
