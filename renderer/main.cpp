#include <vector>
#include <limits>
#include <iostream>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "triangle.h"
#include "transformation.h"
#include "shader.h"
#include "image.h"
#include "platform.h"
#define EPSILON 1e-5

float cur_angle = M_PI/2;
const float rotateAngle = M_PI / 24;
float xzAngle = M_PI / 2;
float yAngle = M_PI / 2;
float distance = (camera - target).norm();
float xzPlaneR = sqrt(pow(distance, 2) - pow((camera - target).y, 2));

std::vector<Model *> models;
enum Mode{general, mdepth, shadow, ambient, wireframe} mode;

/*
 * float_clamp 浮点数区间限制函数，区间为[min,max]
 */
float float_clamp(float f,float min,float max){
	return f < min ? min : (f > max? max:f);
}

/*
 * setXZ 设置摄像机在x,z平面的坐标
 */
void setXZ() {
	camera.x = xzPlaneR * cos(xzAngle);
	camera.z = xzPlaneR * sin(xzAngle);
}
/*
 * setY 设置摄像机在与xz平面垂直的投影在y轴的坐标
 */
void setY() {
	yAngle = float_clamp(yAngle, EPSILON, M_PI - EPSILON);
	camera.y = distance*(float)cos(yAngle);
}

float max_elevation_angle(float *zbuffer, Vec2f p, Vec2f dir) {
	float maxangle = 0;
	for (float t = 0.; t < 1000.; t += 1.) {
		Vec2f cur = p + dir * t;
		if (cur.x >= width || cur.y >= height || cur.x < 0 || cur.y < 0) {
			return maxangle;
		}

		float distance = (p - cur).norm();
		if (distance < 1.f) {
			continue;
		}
		float elevation = zbuffer[int(cur.x) + int(cur.y)*width] - zbuffer[int(p.x) + int(p.y)*width];
		maxangle = std::max(maxangle, atanf(elevation / distance));
	}
	return maxangle;
}

/**
 * 普通渲染器更新函数
 */
void general_update(){
    light_dir = Vec3f(1, 1, 1);
	TGAImage frame(width, height, TGAImage::RGB);
	shadow_lookat(camera, target, up);
	shadow_viewport(width/8, height/8, width*3/4, height*3/4);
	shadow_projection(-1.f/(camera-target).norm());
	light_dir = proj<3>((Projection*ModelView*embed<4>(light_dir, 0.f))).normalize();
	auto *zbuffer = new float[width*height];
	for (int i=width*height-1; i>=0; i--){
		zbuffer[i] = -std::numeric_limits<float>::max();
	}

	for (auto model : models) {
        std::cout << 1 <<std::endl;
		GeneralShader generalshader(ModelView, (Projection*ModelView).invert_transpose());
		Vec4f screen_coords[3];
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				screen_coords[j] = generalshader.vertex(i, j);
			}
			triangle(screen_coords, generalshader, frame, zbuffer);
		}
	}
	frame.flip_vertically(); // to place the origin in the bottom left corner of the image
	frame.write_tga_file("framebuffer.tga");
	frame.clear();
	delete[] zbuffer;
}

/**
 * 深度渲染器更新函数
 */
void depth_update(){
    light_dir = Vec3f(1, 1, 1);
    light_dir.normalize();
	shadowbuffer = new float[width*height];
	for (int i=width*height-1; i>=0; i--){
		shadowbuffer[i] = -std::numeric_limits<float>::max();
	}
	TGAImage frame(width, height, TGAImage::RGB);
	shadow_lookat(light_dir, target, up);
	shadow_viewport(width/8, height/8, width*3/4, height*3/4);
	shadow_projection(0);

	for (auto model : models) {
		DepthShader depthshader;
		Vec4f screen_coords[3];
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				screen_coords[j] = depthshader.vertex(i, j);
			}
			triangle(screen_coords, depthshader, frame, shadowbuffer);
		}
	}
	frame.flip_vertically(); // to place the origin in the bottom left corner of the image
	frame.write_tga_file("framebuffer.tga");
	frame.clear();
	delete[] shadowbuffer;
}

/**
 * 阴影着色器更新函数
 */
void shadow_update(){
    light_dir = Vec3f(1, 1, 1);
    light_dir.normalize();
	auto *zbuffer = new float[width*height];
	shadowbuffer = new float[width*height];

	for (int i=width*height-1; i>=0; i--){
		shadowbuffer[i] = -std::numeric_limits<float>::max();
		zbuffer[i] = -std::numeric_limits<float>::max();
	}

	TGAImage depth(width, height, TGAImage::RGB);
	shadow_lookat(light_dir, target, up);
	shadow_viewport(width/8, height/8, width*3/4, height*3/4);
	shadow_projection(0);

	for (auto model : models) {
		DepthShader depthshader;
		Vec4f screen_coords[3];
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				screen_coords[j] = depthshader.vertex(i, j);
			}
			triangle(screen_coords, depthshader, depth, shadowbuffer);
		}
	}
	//depth.flip_vertically(); // to place the origin in the bottom left corner of the image
	Matrix M = Viewport * Projection*ModelView;

	TGAImage frame(width, height, TGAImage::RGB);
	shadow_lookat(camera, target, up);
	shadow_viewport(width/8, height/8, width*3/4, height*3/4);
	shadow_projection(-1.f/(camera-target).norm());
	//light_dir = proj<3>((Projection*ModelView*embed<4>(light_dir, 0.f))).normalize();

	for (auto model : models) {
		ShadowShader shadowShader(ModelView, (Projection*ModelView).invert_transpose(), M*(Viewport*Projection*ModelView).invert());
		Vec4f screen_coords[3];
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				screen_coords[j] = shadowShader.vertex(i, j);
			}
			triangle(screen_coords, shadowShader, frame, zbuffer);
		}
	}
	frame.flip_vertically(); // to place the origin in the bottom left corner of the image
	frame.write_tga_file("framebuffer.tga");
	frame.clear();
	delete[] zbuffer;
	delete[] shadowbuffer;

}

/**
 * 环境光遮蔽着色器更新函数
 */
void ambient_update() {
    light_dir = Vec3f(1, 1, 1);
	auto *zbuffer = new float[width*height];
	for (int i=width*height-1; i>=0; i--){
		zbuffer[i] = -std::numeric_limits<float>::max();
	}

	TGAImage ambient(width, height, TGAImage::RGB);
	general_lookat(camera, target, up);
	general_viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	general_projection(-1.f / (camera - target).norm());

	for (auto model : models) {
		AmbientShader ambientshader;
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				ambientshader.vertex(i, j);
			}
			triangle(ambientshader._tri_value, ambientshader, ambient, zbuffer);
		}
	}


	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (zbuffer[x + y * width] < -1e5) continue;
			//std::cout << "yes!";
			float total = 0;
			for (float a = 0; a < M_PI * 2 - 1e-4; a += M_PI / 4) {
				total += M_PI / 2 - max_elevation_angle(zbuffer, Vec2f(x, y), Vec2f(cos(a), sin(a)));
			}
			total /= (M_PI / 2) * 8;
			total = pow(total, 100.f);
			ambient.set(x, y, TGAColor(total * 255, total * 255, total * 255));
		}
	}

	ambient.flip_vertically();
	ambient.write_tga_file("framebuffer.tga");
	ambient.clear();
	delete [] zbuffer;
}

/**
 * 线框渲染器更新函数
 */
void wireframe_update(){
    TGAImage wireImage(width,height,TGAImage::RGB);
    general_lookat(camera,target,up);
    general_viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    general_projection(-1.f / (camera - target).norm());

	for (auto model : models) {
		WireFrame wireFrame;
		Vec4f screen_coords[3];
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				screen_coords[j] = wireFrame.vertex(i, j);
			}
			for (int j = 0; j < 3; j++) {
				Vec4f v0 = screen_coords[j];
				Vec4f v1 = screen_coords[(j + 1) % 3];
				line(v0[0], v0[1], v1[0], v1[1], wireImage, TGAColor(255, 255, 255, 255));
			}

		}
	}
	wireImage.flip_vertically();
	wireImage.write_tga_file("framebuffer.tga");
	wireImage.clear();
}

/**
 * 移动视角
 */
void operate_camera(int key) {
	if (key == 0) {
		xzAngle += rotateAngle;
		setXZ();
	} else if (key == 1) {
		xzAngle -= rotateAngle;
		setXZ();
	} else if (key == 10) {
		yAngle += rotateAngle;
		xzPlaneR = distance * sin(yAngle);
		setY();
	} else if (key == 11) {
		yAngle -= rotateAngle;
		xzPlaneR = distance * sin(yAngle);
		setY();
	}
	switch(mode) {
		case general:     general_update();     break;
		case mdepth:      depth_update();       break;
		case shadow:      shadow_update();      break;
		case ambient:     ambient_update();     break;
		case wireframe:   wireframe_update();   break;
	}
}

/**
 * 监听键盘输入
 */
void sanity_check(window_t *window) {
	if (input_key_pressed(window, KEY_A) && mode != mdepth) {
		std::cout << "KEY_A" << std::endl;
		operate_camera(1);
	}
	if (input_key_pressed(window, KEY_D) && mode != mdepth) {
		std::cout << "KEY_D" << std::endl;
		operate_camera(0);
	}
	if (input_key_pressed(window, KEY_S) && mode != mdepth) {
		std::cout << "KEY_S" << std::endl;
		operate_camera(11);
	}
	if (input_key_pressed(window, KEY_W) && mode != mdepth) {
		std::cout << "KEY_W" << std::endl;
		operate_camera(10);
	}
	if (input_key_pressed(window, KEY_1)) {
		std::cout << "KEY_1" << std::endl;
		mode = general;
		general_update();
	}
	if (input_key_pressed(window, KEY_2)) {
		std::cout << "KEY_2" << std::endl;
		mode = mdepth;
		depth_update();
	}
	if (input_key_pressed(window, KEY_3)) {
		std::cout << "KEY_3" << std::endl;
	    mode = shadow;
		shadow_update();
	}
	if (input_key_pressed(window, KEY_4)) {
		std::cout << "KEY_4" << std::endl;
	    mode = ambient;
		ambient_update();
	}
	if (input_key_pressed(window, KEY_5)) {
		std::cout << "KEY_5" << std::endl;
		mode = wireframe;
		wireframe_update();
	}
}


int main(int argc, char** argv) {
	if (2 > argc) {
		std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
		return 1;
	}

	window_t *window;
	image_t *image;
	const char *title = "Viewer";
	mode = general;

	float *zbuffer = new float[width*height];
	for (int i=width*height-1; i>=0; i--){
        zbuffer[i] = -std::numeric_limits<float>::max();
	}

	camera.x = xzPlaneR * cos(cur_angle);
	camera.z = xzPlaneR * sin(cur_angle);
	TGAImage frame(width, height, TGAImage::RGB);
	// 计算MODELVIEW
	shadow_lookat(camera, target, up);
	// 计算VIEWPORT
	shadow_viewport(width/8, height/8, width*3/4, height*3/4);
	// 计算PROJECTION
	shadow_projection(-1.f/(camera-target).norm());
	light_dir = proj<3>((Projection*ModelView*embed<4>(light_dir, 0.f))).normalize();

	for (int m=1; m<argc; m++) {
		model = new Model(argv[m]);
		models.push_back(model);
		GeneralShader generalshader(ModelView, (Projection*ModelView).invert_transpose());
		Vec4f screen_coords[3];
		for (int i = 0; i < model->nfaces(); i++) {
			for (int j = 0; j < 3; j++) {
				screen_coords[j] = generalshader.vertex(i, j);
			}
			triangle(screen_coords, generalshader, frame, zbuffer);
		}
	}
	frame.flip_vertically(); // to place the origin in the bottom left corner of the image
	frame.write_tga_file("framebuffer.tga");
	frame.clear();
	delete [] zbuffer;
	window = window_create(title, width, height);
	while (!window_should_close(window)) {
		image = image_load("framebuffer.tga");
		image_resize(image, width, height);
		window_draw_image(window, image);
		sanity_check(window);
		input_poll_events();
		image_release(image);
	}
	window_destroy(window);

	return 0;
}