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

    Mem_Coord to_mem() const;
};

struct World_Coord {
    V2<float> unwrap;

    World_Coord(V2<float> v2):
        unwrap(v2)
    {}

    Tile_Coord to_tile() const;
};

struct Tile_Region {
    AABB<int> unwrap;

    Tile_Region(AABB<int> region):
        unwrap(region)
    {}
};

struct World_Region {
    AABB<float> unwrap;

    World_Region(AABB<float> region):
        unwrap(region)
    {}

    Tile_Region to_tile() const
    {
        const auto a1 = World_Coord(unwrap.pos).to_tile();
        const auto a2 = World_Coord(unwrap.pos + unwrap.size).to_tile();
        return Tile_Region(AABB(a1.unwrap, a2.unwrap - a1.unwrap + V2(1)));
    }
};

struct Game;

struct Tile_Grid {
    static constexpr size_t QUAD_ROWS = 100;
    static constexpr size_t QUAD_COLS = 100;
    static constexpr size_t ROWS = QUAD_ROWS * 2;
    static constexpr size_t COLS = QUAD_COLS * 2;

    Tile tiles[ROWS][COLS];

    void render(const Game *game, Renderer *renderer) const;
    Tile *get_tile(Mem_Coord coord);
    Tile *get_tile(Tile_Coord coord);
    Tile *get_tile(World_Coord coord);
    const Tile *get_tile(Mem_Coord coord) const;
    const Tile *get_tile(Tile_Coord coord) const;
    const Tile *get_tile(World_Coord coord) const;

    AABB<float> get_tile_hitbox(Tile_Coord coord) const;
    AABB<float> get_tile_hitbox(World_Coord coord) const;

    bool is_there_any_walls_in_region(Tile_Region region) const;
    bool is_there_any_walls_in_region(World_Region region) const;
};

#endif // SOMETHING_TILE_GRID_HPP_
