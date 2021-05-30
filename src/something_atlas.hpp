#ifndef SOMETHING_ATLAS_HPP_
#define SOMETHING_ATLAS_HPP_

#include "./something_math.hpp"

struct Atlas {
    Texture texture;
    GL_Texture gl_texture;
    Dynamic_Array<AABB<float>> uvs;
    Dynamic_Array<V2<int>> sizes;

    static Atlas from_config(const char *file_path, int margin);

    // TODO(#96): introduce a special type Atlas::Index instead of Index<AABB<float>>
    // TODO(#97): introduce a special type in vars.conf for Atlas::Index
    AABB<float> get_uv(Index<AABB<float>> texture_index) const;
    V2<float> get_size(Index<AABB<float>> texture_index, float height) const;
};

#endif // SOMETHING_ATLAS_HPP_
