#ifndef SOMETHING_GAME_HPP_
#define SOMETHING_GAME_HPP_

#include "./something_player.hpp"
#include "./something_triangle_vao.hpp"
#include "./something_camera.hpp"
#include "./something_tile_grid.hpp"
#include "./something_poof.hpp"
#include "./something_projectiles.hpp"
#include "./something_program.hpp"
#include "./something_particles.hpp"
#include "./something_aabb_body.hpp"

struct Game {
    static const size_t AABB_BODIES_CAPACITY = 1024;

    bool quit;

    const Uint8 *keyboard;
    SDL_Window *window;

    // Programs
    Program regular_program;
    Program particle_program;

    // Game Entities
    Atlas atlas;
    Player player;
    Camera camera;
    Tile_Grid tile_grid;
    Poof poof;
    Projectiles projectiles;
    Particles particles;

    V2<Sint32> mouse_window;
    V2<float> mouse_world;

    Index<AABB_Body> camera_follow_body;

    AABB_Body aabb_bodies[AABB_BODIES_CAPACITY];
    size_t aabb_bodies_size;

    Seconds time() const;

    void init(SDL_Window *window);
    void handle_event(const SDL_Event *event);
    void update(Seconds dt);
    void render(Triangle_VAO *triangle_vao, Circle_VAO *circle_vao) const;

    const AABB_Body &get_aabb_body(Index<AABB_Body> body_index) const;
    AABB_Body &get_aabb_body(Index<AABB_Body> body_index);
    Index<AABB_Body> allocate_aabb_body();
};

#endif  // SOMETHING_GAME_HPP_
