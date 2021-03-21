#include <SDL2/SDL.h>
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

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 60;

#include "./something_atlas.cpp"
#include "./something_renderer.cpp"
#include "./something_texture.cpp"
#include "./something_game.cpp"
#include "./something_player.cpp"
#include "./something_main.cpp"
#include "./something_tile_grid.cpp"
