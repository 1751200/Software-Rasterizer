//
// Created by Vodka Soul on 2020-01-12.
//

#ifndef TINYRENDERER_IMAGE_H
#define TINYRENDERER_IMAGE_H

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char *buffer;
} image_t;

image_t *image_create(int width, int height, int channels);
void image_release(image_t *image);
image_t *image_load(const char *filename);
image_t *load_image(std::stringstream &sin);
void image_save(image_t *image, const char *filename);

unsigned char *image_pixel_ptr(image_t *image, int row, int col);
void image_blit_bgr(image_t *src, image_t *dst, int swap_rb);
void image_flip_h(image_t *image);
void image_flip_v(image_t *image);
void image_resize(image_t *image, int width, int height);

#endif //TINYRENDERER_IMAGE_H
