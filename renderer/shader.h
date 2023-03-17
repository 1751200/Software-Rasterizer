#ifndef __SHADER_H__
#define __SHADER_H__

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "scene.h"

struct Shader {
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

struct GeneralShader : public Shader {
	mat<2, 3, float> _uv_value;
	mat<3, 3, float> _tri_value;
	mat<4, 4, float> _M_transform;
	mat<4, 4, float> _MIT_transform;

	GeneralShader(Matrix M, Matrix MIT);
	virtual Vec4f vertex(int iface, int nthvert);
	virtual bool fragment(Vec3f bar, TGAColor &color);
};

struct DepthShader : public Shader {
	mat<3, 3, float> _tri_value;

	virtual Vec4f vertex(int iface, int nthvert);
	virtual bool fragment(Vec3f bar, TGAColor &color);
};

struct ShadowShader : public Shader {
	mat<4, 4, float> _M_transform;
	mat<4, 4, float> _MIT_transform;
	mat<4, 4, float> _Mshadow_transform;
	mat<2, 3, float> _uv_value;
	mat<3, 3, float> _tri_value;

	ShadowShader(Matrix M, Matrix MIT, Matrix MS);
	virtual Vec4f vertex(int iface, int nthvert);
	virtual bool fragment(Vec3f bar, TGAColor &color);
};

struct AmbientShader : public Shader {
	mat<4, 3, float> _tri_value;

	virtual Vec4f vertex(int iface, int nthvert);
	virtual bool fragment(Vec3f bar, TGAColor &color);
};

struct WireFrame : public Shader{

	virtual Vec4f vertex(int iface,int nthvert);
	virtual bool fragment(Vec3f bar, TGAColor &color);
};

#endif //__SHADER_H__
