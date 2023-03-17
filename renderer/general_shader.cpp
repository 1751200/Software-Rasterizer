#include "tgaimage.h"
#include "geometry.h"
#include "transformation.h"
#include "model.h"
#include "shader.h"

GeneralShader::GeneralShader(Matrix M, Matrix MIT) : _M_transform(M), _MIT_transform(MIT), _uv_value(), _tri_value() {}

Vec4f GeneralShader::vertex(int iface, int nthvert) {
	_uv_value.set_col(nthvert, model->uv(iface, nthvert));
	Vec4f gl_Vertex = Viewport*Projection*ModelView*embed<4>(model->vert(iface, nthvert));
	_tri_value.set_col(nthvert, proj<3>(gl_Vertex / gl_Vertex[3]));
	return gl_Vertex;
}

bool GeneralShader::fragment(Vec3f bar, TGAColor &color) {
	Vec2f uv = _uv_value * bar;                 // interpolate uv for the current pixel
	Vec3f n = proj<3>(_MIT_transform*embed<4>(model->normal(uv))).normalize(); // normal
	Vec3f l = proj<3>(_M_transform  *embed<4>(light_dir)).normalize(); // light vector
	Vec3f r = (n*(n*l*2.f) - l).normalize();   // reflected light
	float spec = pow(std::max(r.z, 0.0f), model->specular(uv));
	float diff = std::max(0.f, n*l);
	TGAColor c = model->diffuse(uv);
	for (int i = 0; i < 3; i++) color[i] = std::min<float>(40 + c[i] * (1.2*diff + .6*spec), 255);
	return false;
}