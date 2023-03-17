#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include "geometry.h"

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;
extern int count;
const float depth = 2000.f;

void general_viewport(int x, int y, int w, int h);
void general_projection(float coeff = 0.f); // coeff = -1/c
void general_lookat(Vec3f camera, Vec3f target, Vec3f up);

void shadow_viewport(int x, int y, int w, int h);
void shadow_projection(float coeff = 0.f); // coeff = -1/c
void shadow_lookat(Vec3f camera, Vec3f target, Vec3f up);

#endif //__TRANSFORMATION_H__
