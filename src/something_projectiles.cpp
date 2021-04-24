#include "./something_projectiles.hpp"

void Projectiles::update(Game *game, Seconds dt)
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (states[i] != State::Ded) {
            positions[i] += velocities[i] * dt;
            lifetimes[i] -= dt;

            const auto tile = game->tile_grid.get_tile(World_Coord(positions[i]));
            if (tile && tile->wall) {
                states[i] = State::Ded;
                const auto tri = equilateral_triangle(
                                     positions[i],
                                     RADIUS,
                                     angle_v2(velocities[i]));
                const auto uv = game->atlas.uvs.data[0];
                const auto tri_uv = equilateral_triangle(
                                        uv.pos + uv.size * 0.5f,
                                        uv.size.x * 0.5f,
                                        0.0f);
                explode_triangle(game->poof, tri, COLOR, tri_uv, 1);
            }

            if (lifetimes[i] <= 0) {
                states[i] = State::Ded;
            }
        }
    }
}

void Projectiles::render(Triangle_VAO *triangle_vao) const
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (states[i] == State::Aliv) {
            triangle_vao->fill_triangle(
                equilateral_triangle(
                    positions[i],
                    RADIUS,
                    angle_v2(velocities[i])), COLOR, {});
        }
    }
}

void Projectiles::push(V2<float> pos, V2<float> vel)
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (states[i] == State::Ded) {
            positions[i] = pos;
            velocities[i] = vel;
            states[i] = State::Aliv;
            lifetimes[i] = LIFETIME;
            return;
        }
    }
}
