#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "tgaimage.h"
#include "geometry.h"
#include "transformation.h"
#include "shader.h"
#include "model.h"

/*
 * 求解重心向量
 */
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P);
/*
 * 利用三角形图元进行光栅化
 * 使用包围盒填充法
 */
void triangle(Vec4f *pts, Shader &shader, TGAImage &image, float *zbuffer);
/*
 * 利用三角形图元进行光栅化
 * 使用包围盒填充法
 */
void triangle(mat<4, 3, float> &pts, Shader &shader, TGAImage &image, float *zbuffer);
/*
 * 使用breshem画线算法进行画线
 */
void line(int x0,int y0,int x1,int y1,TGAImage &image, TGAColor color);

#endif //__TRIANGLE_H__
