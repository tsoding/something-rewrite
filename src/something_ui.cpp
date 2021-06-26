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

void Ui::render_button(Renderer *renderer, Atlas *atlas, AABB<float> rect, HSLA color, bool pressed)
{
    const float offset = pressed ? 0.0f : UI_BUTTON_3D_OFFSET;

    renderer->fill_aabb(
        rect,
        HSLA(color.h,
             color.s,
             color.l - 0.25,
             color.a).to_rgba(),
        atlas->get_uv({0}),
        SCREEN_PROGRAM_ASSET);
    renderer->fill_aabb(
        AABB(rect.pos + V2(-1.0f, 1.0f) * V2(offset), rect.size),
        color.to_rgba(),
        atlas->get_uv({0}),
        SCREEN_PROGRAM_ASSET);
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

void Ui::begin_tool_bar(Id tool_cur, float pad)
{
    assert(!tool_bar_button_id.has_value && "Nested tool bars are not allowed, sorry");
    begin_layout(Layout::Kind::Horz, pad);
    tool_bar_button_id = some(tool_cur);
}

bool Ui::tool_bar_button(Renderer *renderer, Atlas *atlas, HSLA color, V2<float> size, Id id)
{

    auto layout = top_layout();
    assert(layout != nullptr && "Can't render a tool bar button outside of any layout");

    const auto pos = layout->available_pos();
    const auto rect = AABB(pos, size);
    assert(tool_bar_button_id.has_value && "Can't render a tool bar button outside of the toolbar");
    const auto pressed = some(id) == tool_bar_button_id;

    switch (update_state(id, rect)) {
    case State::Inactive:
        render_button(renderer, atlas, rect, color, pressed);
        layout->push_widget(size);
        return false;
    case State::Hot:
        color.l += UI_HIGHLIGHT;
        render_button(renderer, atlas, rect, color, pressed);
        layout->push_widget(size);
        return false;
    case State::Active:
        color.s += UI_ACTIVE;
        render_button(renderer, atlas, rect, color, true);
        layout->push_widget(size);
        return false;
    case State::Fired:
        render_button(renderer, atlas, rect, color, true);
        layout->push_widget(size);
        return true;
    default:
        UNREACHABLE(__func__);
    }
}

void Ui::end_tool_bar()
{
    tool_bar_button_id.has_value = false;
    end_layout();
}

bool Ui::button(Renderer *renderer, Atlas *atlas, HSLA color, V2<float> size, Id id)
{
    auto layout = top_layout();
    assert(layout != nullptr && "Can't render a button outside of any layout");

    const auto pos = layout->available_pos();
    const auto rect = AABB(pos, size);

    switch (update_state(id, rect)) {
    case State::Inactive:
        render_button(renderer, atlas, rect, color);
        layout->push_widget(size);
        return false;
    case State::Hot:
        color.l += UI_HIGHLIGHT;
        render_button(renderer, atlas, rect, color);
        layout->push_widget(size);
        return false;
    case State::Active:
        color.s += UI_ACTIVE;
        render_button(renderer, atlas, rect, color, true);
        layout->push_widget(size);
        return false;
    case State::Fired:
        render_button(renderer, atlas, rect, color);
        layout->push_widget(size);
        return true;
    default:
        UNREACHABLE(__func__);
    }
}

void Ui::label(Renderer *renderer, Font *font, String_View text, HSLA color, GLfloat scale)
{
    auto layout = top_layout();
    assert(layout != nullptr && "Can't render a label outside of any layout");

    const auto pos = layout->available_pos();
    const auto size = font->text_size(text.count, scale);

    font->render_text(renderer, text, pos, scale, color.to_rgba());
    layout->push_widget(size);
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

Ui::State Ui::update_state(Id id, AABB<float> rect)
{
    if (active_id == some(id)) {
        if (!mouse_button) {
            active_id.has_value = false;
            if (rect.contains(mouse_pos)) {
                return State::Fired;
            }

            return State::Inactive;
        }
        return State::Active;
    } else if (hot_id == some(id)) {
        if (!rect.contains(mouse_pos)) {
            hot_id.has_value = false;
            return State::Inactive;
        } else if (mouse_button && !active_id.has_value) {
            active_id = some(id);
            return State::Active;
        }

        return State::Hot;
    }

    if (!active_id.has_value && rect.contains(mouse_pos)) {
        hot_id = some(id);
        return State::Hot;
    }

    return State::Inactive;
}

void Ui::tooltip(Renderer *renderer, Atlas *atlas, Font *font, String_View text)
{
    // Tooltip Background
    {
        const auto rect =
            AABB(mouse_pos,
                 font->text_size(text.count, UI_TOOLTIP_SCALE) + V2(UI_TOOLTIP_PADDING * 2.0f));
        renderer->fill_aabb(
            rect,
            UI_TOOLTIP_BACKGROUND,
            atlas->get_uv({0}),
            SCREEN_PROGRAM_ASSET);
    }

    // Tooltip Foreground
    {
        const auto text_pos = mouse_pos + V2(UI_TOOLTIP_PADDING);
        font->render_text(
            renderer,
            text,
            text_pos,
            UI_TOOLTIP_SCALE,
            UI_TOOLTIP_FOREGROUND);
    }
}
