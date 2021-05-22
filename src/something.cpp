#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include "./aids.hpp"

using namespace aids;

typedef float Seconds;
typedef Uint32 Milliseconds;

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

const int SCREEN_WIDTH  = 1920;
const int SCREEN_HEIGHT = 1080;
const int SCREEN_FPS = 60;

template <typename T>
struct Index {
    size_t unwrap;
};

#ifdef SOMETHING_RELEASE
#include "./something_rgba.hpp"
#include "./config_index.hpp"
#else
#include "./config_loader.cpp"
#endif // SOMETHING_RELEASE

#include "./something_atlas.cpp"
#include "./something_texture.cpp"
#include "./something_game.cpp"
#include "./something_player.cpp"
#include "./something_triangle_vao.cpp"
#include "./something_camera.cpp"
#include "./something_main.cpp"
#include "./something_math.cpp"
#include "./something_poof.cpp"
#include "./something_tile_grid.cpp"
#include "./something_projectiles.cpp"
#include "./something_program.cpp"
#include "./something_particles.cpp"
#include "./something_anim.cpp"
#include "./something_aabb_body.cpp"
#include "./something_enemy.cpp"
#include "./something_item.cpp"
#include "./something_renderer.cpp"
#include "./something_shader.cpp"
#include "./something_font.cpp"
