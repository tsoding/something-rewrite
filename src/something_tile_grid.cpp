#include "./something_tile_grid.hpp"

void Tile_Grid::render(const Game *game, Renderer *renderer) const
{
    for (size_t row = 0; row < ROWS; ++row) {
        for (size_t col = 0; col < COLS; ++col) {
            if (tiles[row][col].wall) {
                const auto tile_pos = (V2(row, col) - V2(QUAD_ROWS, QUAD_COLS)).cast_to<float>() * Tile::SIZE;
                const AABB<float> tile_aabb =
                    AABB(tile_pos,
                         V2(Tile::SIZE));
                renderer->fill_rect(tile_aabb, RGBA::RED, game->atlas.uvs.data[0]);
            }
        }
    }
}

Mem_Coord Tile_Coord::to_mem() const
{
    return Mem_Coord(unwrap + V2(Tile_Grid::QUAD_ROWS, Tile_Grid::QUAD_COLS).cast_to<int>());
}

Tile_Coord World_Coord::to_tile() const
{
    return Tile_Coord((unwrap / Tile::SIZE).map(floorf).cast_to<int>());
}

Tile *Tile_Grid::get_tile(Mem_Coord coord)
{
    if (0 <= coord.unwrap.x && coord.unwrap.x < static_cast<int>(COLS) &&
            0 <= coord.unwrap.y && coord.unwrap.y < static_cast<int>(ROWS)) {
        return &tiles[coord.unwrap.y][coord.unwrap.x];
    } else {
        return NULL;
    }
}

Tile *Tile_Grid::get_tile(Tile_Coord coord)
{
    return get_tile(coord.to_mem());
}

Tile *Tile_Grid::get_tile(World_Coord coord)
{
    return get_tile(coord.to_tile());
}

const Tile *Tile_Grid::get_tile(Mem_Coord coord) const
{
    if (0 <= coord.unwrap.x && coord.unwrap.x < static_cast<int>(COLS) &&
            0 <= coord.unwrap.y && coord.unwrap.y < static_cast<int>(ROWS)) {
        return &tiles[coord.unwrap.y][coord.unwrap.x];
    } else {
        return NULL;
    }
}

const Tile *Tile_Grid::get_tile(Tile_Coord coord) const
{
    return get_tile(coord.to_mem());
}

const Tile *Tile_Grid::get_tile(World_Coord coord) const
{
    return get_tile(coord.to_tile());
}

AABB<float> Tile_Grid::get_tile_hitbox(Tile_Coord coord) const
{
    return AABB<float>(
               coord.unwrap.cast_to<float>() * Tile::SIZE,
               V2(Tile::SIZE));
}

AABB<float> Tile_Grid::get_tile_hitbox(World_Coord coord) const
{
    return get_tile_hitbox(coord.to_tile());
}
