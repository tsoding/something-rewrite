#ifndef SOMETHING_ATLAS_HPP_
#define SOMETHING_ATLAS_HPP_

#include "./something_geo.hpp"

// TODO: fix texture bleeding
struct Atlas {
    Texture texture;
    GL_Texture gl_texture;
    Dynamic_Array<AABB<float>> uvs;

    static Atlas from_config(const char *file_path);
};

#endif // SOMETHING_ATLAS_HPP_
