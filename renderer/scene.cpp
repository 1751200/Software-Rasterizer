#include "scene.h"

Model* model = NULL;
float* shadowbuffer = NULL;

Vec3f light_dir(1, 1, 1);
Vec3f       camera(1, 1, 4);
Vec3f    target(0, 0, 0);
Vec3f        up(0, 1, 0);