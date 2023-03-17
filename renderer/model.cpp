#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : _verts(), _faces(), _norms(), _uv(), _diffusemap(), _normalmap(), _specularmap() {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()){
		return;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++){
				iss >> v[i];
			}
			_verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			Vec3f n;
			for (int i = 0; i < 3; i++){
				iss >> n[i];
			}
			_norms.push_back(n);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec2f uv;
			for (int i = 0; i < 2; i++){
				iss >> uv[i];
			}
			_uv.push_back(uv);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<Vec3i> f;
			Vec3i tmp;
			iss >> trash;
			while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
				for (int i = 0; i < 3; i++) {
					tmp[i]--;
				}
				f.push_back(tmp);
			}
			_faces.push_back(f);
		}
	}
	std::cerr << "# v# " << _verts.size() << " f# " << _faces.size() << " vt# " << _uv.size() << " vn# " << _norms.size() << std::endl;
	load_texture(filename, "_diffuse.tga", _diffusemap);
	load_texture(filename, "_nm.tga", _normalmap);
	load_texture(filename, "_spec.tga", _specularmap);
}

Model::~Model() {}

int Model::nverts() {
	return (int)_verts.size();
}

Vec3f Model::get_center() {
	Vec3f center(0, 0, 0);
	for (auto vertex : _verts) {
		for (int i = 0; i < 3; i++) {
			center[i] += vertex[i];
		}
	}
	for (int i = 0; i < 3; i++) {
		center[i] /= nverts();
	}
	return center;
}

int Model::nfaces() {
	return (int)_faces.size();
}

std::vector<int> Model::face(int idx) {
	std::vector<int> face;
	for (int i = 0; i < (int)_faces[idx].size(); i++) face.push_back(_faces[idx][i][0]);
	return face;
}

Vec3f Model::vert(int i) {
	return _verts[i];
}

Vec3f Model::vert(int i_face, int nth_vert) {
	return _verts[_faces[i_face][nth_vert][0]];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img.flip_vertically();
	}
}

TGAColor Model::diffuse(Vec2f uvf) {
	Vec2i uv(uvf[0] * _diffusemap.get_width(), uvf[1] * _diffusemap.get_height());
	return _diffusemap.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f uvf) {
	Vec2i uv(uvf[0] * _normalmap.get_width(), uvf[1] * _normalmap.get_height());
	TGAColor c = _normalmap.get(uv[0], uv[1]);
	Vec3f res;
	for (int i = 0; i < 3; i++){
		res[2 - i] = (float)c[i] / 255.f*2.f - 1.f;
	}

	return res;
}

Vec2f Model::uv(int i_face, int nth_vert) {
	return _uv[_faces[i_face][nth_vert][1]];
}

float Model::specular(Vec2f uvf) {
	Vec2i uv(uvf[0] * _specularmap.get_width(), uvf[1] * _specularmap.get_height());
	return _specularmap.get(uv[0], uv[1])[0] / 1.f;
}

Vec3f Model::normal(int i_face, int nth_vert) {
	int idx = _faces[i_face][nth_vert][2];
	return _norms[idx].normalize();
}

