#ifndef SOMETHING_ITEM_HPP_
#define SOMETHING_ITEM_HPP_

struct Item
{
    enum class State {
        Ded,
        Alive,
    };

    V2<float> pos;
    State state;
    Index<AABB<float>> uv_index;

    static Item make_tea(V2<float> pos);

    void render(const Game *game, Triangle_VAO *triangle_vao) const;
    void update(Game *game, Seconds dt);
};

#endif // SOMETHING_ITEM_HPP_
