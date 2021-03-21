#include "./something_tile_grid.hpp"

void Tile_Grid::render(const Game *, Renderer *renderer) const
{
    for (size_t row = 0; row < ROWS; ++row) {
        for (size_t col = 0; col < COLS; ++col) {
            if (tiles[row][col].wall) {
                const auto tile_pos = (V2(row, col) - V2(QUAD_ROWS, QUAD_COLS)).cast_to<float>() * Tile::SIZE;
                const AABB<float> tile_aabb =
                    AABB(tile_pos,
                         V2(Tile::SIZE));
                renderer->fill_rect(tile_aabb, RGBA::RED);
            }
        }
    }
}

Tile &Tile_Grid::get_tile(V2<int> world_coord)
{
    const auto mem_coord = world_coord + V2(QUAD_ROWS, QUAD_COLS).cast_to<int>();
    assert(0 <= mem_coord.x && mem_coord.x < static_cast<int>(COLS));
    assert(0 <= mem_coord.y && mem_coord.y < static_cast<int>(ROWS));
    return tiles[mem_coord.y][mem_coord.x];
}
