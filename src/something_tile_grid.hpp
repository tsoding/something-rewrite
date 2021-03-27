#ifndef SOMETHING_TILE_GRID_HPP_
#define SOMETHING_TILE_GRID_HPP_

struct Tile {
    static constexpr float SIZE = 64.0f;

    bool wall;
};

struct Mem_Coord {
    V2<int> unwrap;

    Mem_Coord(V2<int> v2):
        unwrap(v2)
    {}
};

struct Tile_Coord {
    V2<int> unwrap;

    Tile_Coord(V2<int> v2):
        unwrap(v2)
    {}
};

struct World_Coord {
    V2<float> unwrap;

    World_Coord(V2<float> v2):
        unwrap(v2)
    {}
};

struct Tile_Grid {
    static constexpr size_t QUAD_ROWS = 10;
    static constexpr size_t QUAD_COLS = 10;
    static constexpr size_t ROWS = QUAD_ROWS * 2;
    static constexpr size_t COLS = QUAD_COLS * 2;

    Tile tiles[ROWS][COLS];

    void render(const Game *game, Renderer *renderer) const;
    Tile &get_tile(Mem_Coord coord);
    Tile &get_tile(Tile_Coord coord);
    Tile &get_tile(World_Coord coord);
};

#endif // SOMETHING_TILE_GRID_HPP_
