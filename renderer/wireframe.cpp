//
// Created by Vodka Soul on 2020-01-14.
//

#include "tgaimage.h"
#include "geometry.h"
#include "transformation.h"
#include "model.h"
#include "shader.h"


Vec4f WireFrame::vertex(int iface,int nthvert){
    Vec4f gl_Vertex = Viewport*ModelView*embed<4>(model->vert(iface, nthvert));
    return gl_Vertex;
}

bool WireFrame::fragment(Vec3f bar, TGAColor &color) {
    return false;
}


