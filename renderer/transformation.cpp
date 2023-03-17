#include "transformation.h"

Matrix ModelView;
Matrix Viewport;
Matrix Projection;
int count = 0;

void general_viewport(int x, int y, int w, int h) {
	Viewport = Matrix::identity();
	Viewport[0][0] = w / 2.f;
	Viewport[1][1] = h / 2.f;
	Viewport[2][2] = 0;
	Viewport[0][3] = x + w / 2.f;
	Viewport[1][3] = y + h / 2.f;
	Viewport[2][3] = 1.f;
}

void general_projection(float coeff) {
	Projection = Matrix::identity();
	Projection[3][2] = coeff;
}

void general_lookat(Vec3f camera, Vec3f target, Vec3f up) {
	Vec3f z = (camera - target).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	Matrix R = Matrix::identity();
	Matrix Move = Matrix::identity();
	for (int i = 0; i < 3; i++) {
		R[0][i] = x[i];
		R[1][i] = y[i];
		R[2][i] = z[i];
		Move[i][3] = -target[i];
	}
	ModelView = R * Move;
}

void shadow_viewport(int x, int y, int w, int h) {
	Viewport = Matrix::identity();
	Viewport[0][0] = w / 2.f;
	Viewport[1][1] = h / 2.f;
	Viewport[2][2] = depth / 2.f;
	Viewport[0][3] = x + w / 2.f;
	Viewport[1][3] = y + h / 2.f;
	Viewport[2][3] = depth / 2.f;
}

void shadow_projection(float coeff) {
	Projection = Matrix::identity();
	Projection[3][2] = coeff;
}

void shadow_lookat(Vec3f camera, Vec3f target, Vec3f up) {
	Vec3f z = (camera - target).normalize();
	Vec3f x = cross(up, z).normalize();
	Vec3f y = cross(z, x).normalize();
	ModelView = Matrix::identity();
	for (int i = 0; i < 3; i++) {
		ModelView[0][i] = x[i];
		ModelView[1][i] = y[i];
		ModelView[2][i] = z[i];
		ModelView[i][3] = -target[i];
	}
}