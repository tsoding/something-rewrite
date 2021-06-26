#ifndef SOMETHING_UI_HPP_
#define SOMETHING_UI_HPP_

#include "./something_math.hpp"

struct Ui {
    const static size_t LAYOUTS_CAPACITY = 256;

    enum class State {
        Inactive,
        Hot,
        Active,
        Fired
    };

    struct Layout {
        enum class Kind {
            Horz,
            Vert
        };

        Kind kind;
        V2<float> pos;
        V2<float> size;
        float pad;

        V2<float> available_pos() const;
        void push_widget(V2<float> size);
    };

    using Id = size_t;

    State update_state(Id id, AABB<float> rect);

    V2<float> mouse_pos;
    bool mouse_button;

    Maybe<Id> active_id;
    Maybe<Id> hot_id;
    Maybe<Id> tool_bar_button_id;
    Layout layouts[LAYOUTS_CAPACITY];
    size_t layouts_count;

    void push_layout(Layout layout);
    Layout pop_layout();
    Layout *top_layout();

    void render_button(Renderer *renderer, Atlas *atlas, AABB<float> rect, HSLA color, bool pressed = false);

    void begin(V2<float> pos, float pad);
    void begin_layout(Layout::Kind kind, float pad);

    void begin_tool_bar(Id tool_cur, float pad);
    bool tool_bar_button(Renderer *renderer, Atlas *atlas, HSLA color, V2<float> size, Id id);
    void end_tool_bar();

    bool button(Renderer *renderer, Atlas *atlas, HSLA color, V2<float> size, Id id);
    void label(Renderer *renderer, Font *font, String_View text, HSLA color, GLfloat scale);
    bool screen(Id id);
    void end_layout();
    void end();
};

#endif // SOMETHING_UI_HPP_
