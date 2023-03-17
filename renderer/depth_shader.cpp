#include "tgaimage.h"
#include "geometry.h"
#include "transformation.h"
#include "model.h"
#include "shader.h"

Vec4f DepthShader::vertex(int iface, int nthvert) {
	Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
	gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;          // transform it to screen coordinates
	_tri_value.set_col(nthvert, proj<3>(gl_Vertex / gl_Vertex[3]));
	return gl_Vertex;
}

bool DepthShader::fragment(Vec3f bar, TGAColor &color) {
	Vec3f p = _tri_value * bar;
	color = TGAColor(255, 255, 255)*(p.z / depth);
	return false;
}