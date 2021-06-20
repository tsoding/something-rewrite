#include "./something_ui.hpp"

V2<float> Ui::Layout::available_pos() const
{
    switch (kind) {
    case Kind::Horz:
        return pos + (size + V2(pad)) * V2(1.0f, 0.0f);
    case Kind::Vert:
        return pos + (size + V2(pad)) * V2(0.0f, 1.0f);
    default:
        UNREACHABLE(__func__);
    }
}

void Ui::Layout::push_widget(V2<float> widget_size)
{
    switch (kind) {
    case Kind::Horz:
        size.x = size.x + widget_size.x + pad;
        size.y = max(size.y, widget_size.y);
        break;
    case Kind::Vert:
        size.x = max(size.x, widget_size.x);
        size.y = size.y + widget_size.y + pad;
        break;
    default:
        UNREACHABLE(__func__);
    }
}

void Ui::push_layout(Layout layout)
{
    assert(layouts_count < LAYOUTS_CAPACITY);
    layouts[layouts_count++] = layout;
}

Ui::Layout Ui::pop_layout()
{
    assert(layouts_count > 0);
    return layouts[--layouts_count];
}

Ui::Layout *Ui::top_layout()
{
    if (layouts_count > 0) {
        return &layouts[layouts_count - 1];
    }

    return NULL;
}

void Ui::begin(V2<float> pos, float pad)
{
    Layout layout = {};
    layout.kind = Layout::Kind::Horz;
    layout.pos = pos;
    layout.size = {};
    layout.pad = pad;
    push_layout(layout);
}

void Ui::begin_layout(Layout::Kind kind, float pad)
{
    auto prev = top_layout();
    assert(prev != nullptr);

    Layout next = {};
    next.kind = kind;
    next.pos = prev->available_pos();
    next.size = {};
    next.pad = pad;
    push_layout(next);
}

bool Ui::button(Renderer *renderer, Atlas *atlas, RGBA color, V2<float> size, Id id)
{
    auto layout = top_layout();
    assert(layout != nullptr);

    const auto pos = layout->available_pos();
    const auto rect = AABB(pos, size);

    bool click = false;

    if (active_id == some(id)) {
        color = color.invert();

        if (!mouse_button) {
            active_id.has_value = false;

            if (rect.contains(mouse_pos)) {
                click = true;
            }
        }
    } else if (hot_id == some(id)) {
        color.r = min(color.r + UI_HIGHLIGHT, 1.0f);
        color.g = min(color.g + UI_HIGHLIGHT, 1.0f);
        color.b = min(color.b + UI_HIGHLIGHT, 1.0f);

        if (!rect.contains(mouse_pos)) {
            hot_id.has_value = false;
        } else if (mouse_button && !active_id.has_value) {
            active_id = some(id);
        }
    } else {
        if (!active_id.has_value && rect.contains(mouse_pos)) {
            hot_id = some(id);
        }
    }

    renderer->fill_aabb(rect, color, atlas->get_uv({0}), SCREEN_PROGRAM_ASSET);

    layout->push_widget(size);

    return click;
}

bool Ui::screen(Ui::Id id)
{
    bool click = false;
    if (active_id == some(id)) {
        if (!mouse_button) {
            active_id.has_value = false;
            click = true;
        }
    } else {
        if (mouse_button && !active_id.has_value) {
            active_id = some(id);
        }
    }

    return click;
}

void Ui::end_layout()
{
    Layout child = pop_layout();
    Layout *parent = top_layout();
    assert(parent != nullptr);
    parent->push_widget(child.size);
}

void Ui::end()
{
    pop_layout();
}
