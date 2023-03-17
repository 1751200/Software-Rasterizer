//
// Created by Vodka Soul on 2020-01-12.
//

#ifndef TINYRENDERER_PLATFORM_H
#define TINYRENDERER_PLATFORM_H

#include <sstream>
#include "image.h"
#include "tgaimage.h"

typedef struct window window_t;
typedef enum {KEY_A, KEY_D, KEY_S, KEY_W, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_PLUS, KEY_MINUS, KEY_NUM} keycode_t;
typedef enum {BUTTON_L, BUTTON_R, BUTTON_NUM} button_t;

window_t *window_create(const char *title, int width, int height);
void window_destroy(window_t *window);
int window_should_close(window_t *window);
void window_draw_image(window_t *window, image_t *image);

void input_poll_events(void);
int input_key_pressed(window_t *window, keycode_t key);
int input_button_pressed(window_t *window, button_t button);
void input_query_cursor(window_t *window, int *xpos, int *ypos);
double input_get_time(void);

#endif //TINYRENDERER_PLATFORM_H
