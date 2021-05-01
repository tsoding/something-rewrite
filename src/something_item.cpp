#include "./something_item.hpp"

void Item::render(const Game *game, Triangle_VAO *triangle_vao) const
{
    if (state == State::Alive) {
        const float ITEM_SIZE = 69.0f;
        triangle_vao->fill_aabb(
            AABB(pos - V2(ITEM_SIZE * 0.5f), V2(ITEM_SIZE)),
            RGBA(1.0f),
            game->atlas.get_uv(uv_index).flip_vertically());
    }
}

void Item::update(Game*, Seconds)
{
    if (state == State::Alive) {
    }
}

Item Item::make_tea(V2<float> pos)
{
    Item item = {};
    item.pos = pos;
    item.state = State::Alive;
    item.uv_index = {11};
    return item;
}
