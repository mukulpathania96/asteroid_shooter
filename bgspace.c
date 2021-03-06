#include <math.h>
#include "bgspace.h"
#include <stdio.h>
#include <stdlib.h>
#include<allegro5/allegro_primitives.h>

void must_init(bool, const char *);
//error checking function defined in main.c


/*These three functions handle the drawing of stars in the background.*/
static void init_star();
static void update_star();
static void draw_star();

/*These functions handle all the aspects of the planets*/
static void init_planets();
static void trigger_planet();
static void create_planet();
static void draw_planets();
static void update_planets();
static void destroy_planets();


ALLEGRO_TRANSFORM BG_SPACE_TRANSFORM;
/*This function is used to initialise all the components that are drawn 
 *in the background of the game.
 *This function will just call the init functions(declared below) of the 
 *different components of the backgorund like comets, stars, planets.
 *Only this function will be caleed in the main() function instead of 
 *all the init functions.*/
void init_bgspace(){
    init_planets();
    init_star();
}

/*This function is used to update all the components of the backgorund.
 *It does not do anything on it's own but call the update function of 
 *the different components that do the work.*/
void update_bgspace(){
    update_planets();
    update_star();
}

/*This function will trigger the creation of all the components of the
 *background. It just calls the trigger function of the different components.*/
void trigger_bgspace(){
    trigger_planet();
}

/*Draw all the components of the background*/
void draw_bgspace(){
    draw_star();
    draw_planets();
}

/*This function will destroy all the resources in use by the background space.*/
void destroy_bgspace(){
    destroy_planets();
}


ALLEGRO_BITMAP *PLANET_BITMAP, *PLANETS[5];
PLANET planets[MAX_PLANETS];
int planets_on_screen;

/*This function loads the planet.png bitmap, extract the different planets from
 *the bitmap, redraw them on diferent bitmaps according to our needs and enable 
 *us to use those bitmaps.
 *It also initialises the planets array and sets planets_on_screen to 0.*/
static void init_planets(){
    PLANET_BITMAP = al_load_bitmap("resources/planet.png");
    must_init(PLANET_BITMAP, "PLANET_BITMAP");
    PLANETS[0] = al_create_bitmap(132, 132);//icy-blue planet
    must_init(PLANETS[0], "PLANET[]");
    PLANETS[1] = al_create_bitmap(135, 135);//yellowish planet
    must_init(PLANETS[1], "PLANET[]");
    PLANETS[2] = al_create_bitmap(104, 104);//violet planet
    must_init(PLANETS[2], "PLANET[]");
    PLANETS[3] = al_create_bitmap(130, 130);//red planet
    must_init(PLANETS[3], "PLANET[]");
    PLANETS[4] = al_create_bitmap(78, 78);//candy planet
    must_init(PLANETS[4], "PLANET[]");

    al_set_target_bitmap(PLANETS[0]);
    al_draw_scaled_bitmap(PLANET_BITMAP, 500, 70, 132, 132, 0, 0, 132, 132, 0);
    al_set_target_bitmap(PLANETS[1]);
    al_draw_scaled_bitmap(PLANET_BITMAP, 497, 209, 135, 135, 0, 0, 135, 135, 0);
    al_set_target_bitmap(PLANETS[2]);
    al_draw_scaled_bitmap(PLANET_BITMAP, 367, 233, 104, 104, 0, 0, 104, 104, 0);
    al_set_target_bitmap(PLANETS[3]);
    al_draw_scaled_bitmap(PLANET_BITMAP, 534, 374, 86, 86, 0, 0, 86, 86, 0);
    al_set_target_bitmap(PLANETS[4]);
    al_draw_scaled_bitmap(PLANET_BITMAP, 540, 467, 78, 78, 0, 0, 78, 78, 0);

    al_set_target_bitmap(al_get_backbuffer(disp));
    for(int i = 0; i < MAX_PLANETS; i++)
        planets[i].gone = true;

    planets_on_screen = 0;
}

static void create_planet(){
    static int bitmap_num = 0;
    float width, height;
    for(int i = 0; i < MAX_PLANETS; i++){
        if(!planets[i].gone)
            continue;
        planets[i].speed = 1;
        planets[i].bmp = PLANETS[bitmap_num++];
        bitmap_num %= 5;
        width = al_get_bitmap_width(planets[i].bmp) / 2;
        height = al_get_bitmap_height(planets[i].bmp) / 2;
        planets[i].x = (i * (SCREEN_WIDTH / 3)) + RAND_DOUBLE_RANGE(width, (SCREEN_WIDTH / 3) - width);
        planets[i].y = -height;
        planets[i].gone = false;
        planets_on_screen++;
        return;
    }

}
static void trigger_planet(){
    int i = rand() % PLANET_SPAWN_RATE;
    if((i == 0) && (planets_on_screen < MAX_PLANETS))
        create_planet();
}

static void update_planets(){
    float height;
    for(int i = 0; i < MAX_PLANETS; i++){
        if(planets[i].gone)
            continue;
        planets[i].y += planets[i].speed;
        height = al_get_bitmap_height(planets[i].bmp) / 2;
        if(planets[i].y - height > SCREEN_HEIGHT){
            planets[i].gone = true;
            planets_on_screen--;
        }
    }
}


static void draw_planets(){
    al_build_transform(&BG_SPACE_TRANSFORM, 0, 0, 1, 1, 0);
    al_use_transform(&BG_SPACE_TRANSFORM);
    float width, height;
    for(int i = 0; i < MAX_PLANETS; i++){
        if(planets[i].gone)
            continue;
        height = al_get_bitmap_height(planets[i].bmp) / 2;
        width = al_get_bitmap_width(planets[i].bmp) / 2;
        al_draw_bitmap(planets[i].bmp, planets[i].x - width, planets[i].y - height, 0);
    }

}

static void destroy_planets(){
    al_destroy_bitmap(PLANET_BITMAP);
    for(int i = 0; i < 5; i++)
        al_destroy_bitmap(PLANETS[i]);
}


STAR stars[STAR_COUNT];
static void init_star(){
    int x = 1;

    for(int i=0; i < STAR_COUNT ; i++, x += 2){
        stars[i].x = x;
        stars[i].y = rand() % SCREEN_HEIGHT ;
        stars[i].speed = RAND_DOUBLE_RANGE(0.1, 1);
    }
}

static void update_star(){

    for(int i=0; i < STAR_COUNT; i++){

        stars[i].y += stars[i].speed;
        if (stars[i].y > SCREEN_HEIGHT){
            stars[i].y = 0;
        }
    }
}

static void draw_star(){
    al_build_transform(&BG_SPACE_TRANSFORM, 0, 0, 1, 1, 0);
    al_use_transform(&BG_SPACE_TRANSFORM);
    float l;

    for(int i=0; i < STAR_COUNT; i++){
        l = stars[i].speed;
        al_draw_pixel(stars[i].x , stars[i].y, al_map_rgb(0, l * 255, l * 255));
    }
}

