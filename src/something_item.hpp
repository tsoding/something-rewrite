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

    void render(const Game *game, Renderer *renderer) const;
};

#endif // SOMETHING_ITEM_HPP_
