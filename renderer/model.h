#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
	std::vector<Vec3f> _verts;
	std::vector<std::vector<Vec3i> > _faces;
	std::vector<Vec3f> _norms;
	std::vector<Vec2f> _uv;
	TGAImage _diffusemap;
	TGAImage _normalmap;
	TGAImage _specularmap;
	void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
	Model(const char *filename);
	~Model();
	int nverts();
	Vec3f get_center();
	int nfaces();
	Vec3f normal(int i_face, int nth_vert);
	Vec3f normal(Vec2f uv);
	Vec3f vert(int i);
	Vec3f vert(int i_face, int nth_vert);
	Vec2f uv(int i_face, int nth_vert);
	TGAColor diffuse(Vec2f uv);
	float specular(Vec2f uv);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__

