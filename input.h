#ifndef FILE_INPUT_SEEN
#define FILE_INPUT_SEEN 1

#include <allegro5/allegro5.h>

extern bool key[ALLEGRO_KEY_MAX];
void init_input();
void keyboard_update(ALLEGRO_EVENT* event);

#endif //input.h
