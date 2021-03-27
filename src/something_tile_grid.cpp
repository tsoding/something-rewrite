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

Tile &Tile_Grid::get_tile(Mem_Coord coord)
{
    assert(0 <= coord.unwrap.x && coord.unwrap.x < static_cast<int>(COLS));
    assert(0 <= coord.unwrap.y && coord.unwrap.y < static_cast<int>(ROWS));
    return tiles[coord.unwrap.y][coord.unwrap.x];
}

Tile &Tile_Grid::get_tile(Tile_Coord coord)
{
    return get_tile(Mem_Coord(coord.unwrap + V2(QUAD_ROWS, QUAD_COLS).cast_to<int>()));
}

Tile &Tile_Grid::get_tile(World_Coord coord)
{
    return get_tile(Tile_Coord((coord.unwrap / Tile::SIZE).cast_to<int>()));
}
