#include "triangle.h"

/*
 * 判断点是否在三角形内
 */
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
	Vec3f triangle[2];
	for (int i = 2; i--; ) {
		triangle[i][0] = C[i] - A[i];
		triangle[i][1] = B[i] - A[i];
		triangle[i][2] = A[i] - P[i];
	}
	Vec3f u = cross(triangle[0], triangle[1]);
	if (std::abs(u[2]) > 1e-2){
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}

	return Vec3f(-1, -1, -1);
}

void triangle(Vec4f *pts, Shader &shader, TGAImage &image, float *zbuffer) {
	// 计算包围盒的最小值和最大值
	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::min(bboxmin[j], pts[i][j] / pts[i][3]);
			bboxmax[j] = std::max(bboxmax[j], pts[i][j] / pts[i][3]);
		}
	}
	Vec2i P;
	TGAColor color;
	// 遍历判断点是否在三角形内
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3f c = barycentric(proj<2>(pts[0] / pts[0][3]), proj<2>(pts[1] / pts[1][3]), proj<2>(pts[2] / pts[2][3]), proj<2>(P));
			Vec3f clip = Vec3f(c[0]/pts[0][3],c[1]/pts[1][3],c[2]/pts[2][3]);
			clip = clip / (clip[0]+clip[1]+clip[2]);
			float frag_depth = Vec3f(pts[0][2],pts[1][2],pts[2][2]) * clip;

			if (c.x < 0 || c.y < 0 || c.z<0 || zbuffer[P.x + P.y*image.get_width()]>frag_depth) {
				continue;
			}
			bool discard = shader.fragment(c, color);
			if (!discard) {
				zbuffer[P.x + P.y*image.get_width()] = frag_depth;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void triangle(mat<4, 3, float> &clipc, Shader &shader, TGAImage &image, float *zbuffer) {
	mat<3, 4, float> pts = (Viewport*clipc).transpose();
	mat<3, 2, float> pts2;
	for (int i = 0; i < 3; i++){
		pts2[i] = proj<2>(pts[i] / pts[i][3]);
	}

	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::max(0.f, std::min(bboxmin[j], pts2[i][j]));
			bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts2[i][j]));
		}
	}
	Vec2i P;
	TGAColor color;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			Vec3f screen_coords = barycentric(pts2[0], pts2[1], pts2[2], P);
			Vec3f clip = Vec3f(screen_coords.x / pts[0][3], screen_coords.y / pts[1][3], screen_coords.z / pts[2][3]);
			clip = clip / (clip.x + clip.y + clip.z);
			float frag_depth = clipc[2] * clip;
			if (screen_coords.x < 0 || screen_coords.y < 0 || screen_coords.z<0 || zbuffer[P.x + P.y*image.get_width()]>frag_depth)
            {
			    continue;
            }
			bool discard = shader.fragment(clip, color);
			if (!discard) {
				zbuffer[P.x + P.y*image.get_width()] = frag_depth;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void line(int x0,int y0,int x1,int y1, TGAImage &image, TGAColor color){
	bool isSteep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		isSteep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x=x0; x<=x1; x++) {
		float t = (x-x0)/(float)(x1-x0);
		int y = y0*(1.-t) + y1*t;
		if (isSteep) {
			image.set(y, x, color);
		} else {
			image.set(x, y, color);
		}
	}
}