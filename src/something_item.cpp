#include "./something_item.hpp"

void Item::render(const Game *game, Renderer *renderer) const
{
    if (state == State::Alive) {
        const auto offset = V2(0.0f, sinf(ITEM_IDLE_FREQUENCY * game->time())) * ITEM_IDLE_AMPLITUDE;

        renderer->fill_aabb(
            AABB(pos + offset - V2(ITEM_SIZE * 0.5f),
                 V2(ITEM_SIZE)),
            RGBA(1.0f),
            game->atlas.get_uv(uv_index).flip_vertically());
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
