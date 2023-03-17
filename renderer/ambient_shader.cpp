#include "tgaimage.h"
#include "geometry.h"
#include "transformation.h"
#include "model.h"
#include "shader.h"

Vec4f AmbientShader::vertex(int iface, int nthvert) {
	Vec4f gl_Vertex = Projection * ModelView*embed<4>(model->vert(iface, nthvert));
	_tri_value.set_col(nthvert, gl_Vertex);
	return gl_Vertex;
}

bool AmbientShader::fragment(Vec3f bar, TGAColor &color) {
	color = TGAColor(0, 0, 0);
	return false;
}
