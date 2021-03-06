#include "main.h"
#include "FX.h"
#include "input.h"
#include "blast.h"
#include <stdio.h>
#include <stdlib.h>
#include "spaceship.h"
#include "sounds.h"
#include "bgspace.h"
#include "asteroid.h"
#include "menusystem.h"
#include "HUD.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}


ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_EVENT event;
bool done = false;
bool menu = false;


/*This function calls all the init functions.*/
void init_main(){
    init_menusystem();
    init_input();  //To handle keyboard events
    audio_init();
    init_asteroids(); //initialise asteroids for use.
    init_ship();   //initialize ship
    init_blasts(); //initialise blasts
    FX_init();
    init_bgspace();//initialise all the background elements.
    init_HUD(); 
}


/*This function is going to be super-useful in other files.*/
void destroy_main(){
    destroy_audio();
    destroy_ship();
    destroy_asteroids();
    destroy_FX();
    destroy_bgspace();
    destroy_blasts();
    destroy_menusystem();
    al_destroy_display(disp);
    exit(0);
}

/*This function is to initialise the spaceship again when the user uses the
 *START GAME option.
 */
void reload_ship(){
    destroy_ship();
    init_ship();

}

void game_loop(){
    
    timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    queue = al_create_event_queue();
    must_init(queue, "queue");
    
    font = al_create_builtin_font();
    must_init(font, "font");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    
    play_background_music();
    done = false;
    bool redraw = true;
    int choice = 0;
    menu = false;
    bool display_closed = false;
    al_start_timer(timer);
    
    while(1){
        
        al_wait_for_event(queue, &event);

        switch(event.type){

            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                
                if(key[ALLEGRO_KEY_ESCAPE])
                    menu = true;
                
                if(menu){
                    if(key[ALLEGRO_KEY_UP])
                        choice--;
                    if(key[ALLEGRO_KEY_DOWN])
                        choice++;
                    if(choice < 0)
                        choice = 1;
                    if(choice > 1)
                        choice = 0;

                    if(key[ALLEGRO_KEY_ENTER])
                        pause_menu[choice].handler();
                    
                    //This break is intentional
                    break;
                }
                update_bgspace(); //update the background.
                check_and_handle_collisions();//check for collision between ship, blasts and asteroids and handle them if any.
                asteroid_trigger(); //create new asteroids.
                update_asteroids(); //update all the asteroids on screen.
                ship_update(ship); //update ship
                blast_trigger(); //create blasts
                update_blasts();  //update the blasts on the screen.
                FX_update();
                trigger_bgspace();
                update_HUD();

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                display_closed = true;
                done = true;
                break;
        }

        keyboard_update(&event);

        if(menu)
            handle_mouse_hover_and_click(&event, pause_menu, 2, &choice);

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_bgspace();
            FX_draw();

            draw_all_asteroids();

            draw_ship(ship); // draws spaceship

            draw_all_blasts(); //draws all the blasts
            draw_HUD();
            
            if(menu)
                draw_pause_menu(choice);

            al_flip_display();

            redraw = false;
        }
    }
    stop_background_music();
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    /*If the display is closed while in game loop then destroy main should be called here*/
    if(display_closed)
        destroy_main();
    return;

}


int main(){

    srandom(time(0));
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(),"mouse");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    must_init(disp, "display");

    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_image_addon(), "image-addon");

    init_main();

    welcome_screen();

    destroy_main();
    return 0;
}
