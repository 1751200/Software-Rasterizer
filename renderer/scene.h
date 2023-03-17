#ifndef __SCENE_H__
#define __SCENE_H__

#include "geometry.h"
#include "model.h"

const int width = 800;
const int height = 800;

extern Model* model;
extern float* shadowbuffer;

extern Vec3f light_dir;
extern Vec3f       camera;
extern Vec3f    target;
extern Vec3f        up;

#endif //__SCENE_H__
