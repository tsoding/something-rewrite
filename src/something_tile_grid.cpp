#include "./something_tile_grid.hpp"

void Tile_Grid::render(const Game *game, Triangle_VAO *triangle_vao) const
{
    const size_t GRASS_INDEX = 8;
    const size_t DIRT_INDEX = 9;

    for (size_t y = 0; y < ROWS; ++y) {
        for (size_t x = 0; x < COLS; ++x) {
            if (tiles[y][x].wall) {
                const auto tile_coord = Mem_Coord(V2(x, y).cast_to<int>()).to_tile();
                const AABB<float> tile_aabb =
                    AABB(tile_coord.to_world().unwrap,
                         V2(Tile::SIZE));
                const auto tile = get_tile(Tile_Coord(tile_coord.unwrap + V2(0, 1)));
                if (tile && tile->wall) {
                    triangle_vao->fill_rect(
                        tile_aabb,
                        RGBA::WHITE(),
                        game->atlas.uvs.data[DIRT_INDEX].flip_vertically());
                } else {
                    triangle_vao->fill_rect(
                        tile_aabb,
                        RGBA::WHITE(),
                        game->atlas.uvs.data[GRASS_INDEX].flip_vertically());
                }
            }
        }
    }
}

Tile_Coord Mem_Coord::to_tile() const
{
    return Tile_Coord(unwrap.cast_to<int>() - V2(Tile_Grid::QUAD_COLS, Tile_Grid::QUAD_ROWS).cast_to<int>());
}

Mem_Coord Tile_Coord::to_mem() const
{
    return Mem_Coord(unwrap + V2(Tile_Grid::QUAD_ROWS, Tile_Grid::QUAD_COLS).cast_to<int>());
}

World_Coord Tile_Coord::to_world() const
{
    return unwrap.cast_to<float>() * Tile::SIZE;
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

bool Tile_Grid::is_there_any_walls_in_region(Tile_Region region) const
{
    const auto a1 = region.unwrap.pos;
    const auto a2 = a1 + region.unwrap.size;

    for (int y = a1.y; y < a2.y; ++y) {
        for (int x = a1.x; x < a2.x; ++x) {
            const auto tile = get_tile(Tile_Coord(V2(x, y)));
            if (tile && tile->wall) {
                return true;
            }
        }
    }

    return false;
}

bool Tile_Grid::is_there_any_walls_in_region(World_Region region) const
{
    return is_there_any_walls_in_region(region.to_tile());
}
