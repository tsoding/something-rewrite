#ifndef SOMETHING_UI_HPP_
#define SOMETHING_UI_HPP_

#include "./something_math.hpp"

struct Ui {
    const static size_t LAYOUTS_CAPACITY = 256;

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

    V2<float> mouse_pos;
    bool mouse_button;

    Maybe<Id> active_id;
    Maybe<Id> hot_id;
    Layout layouts[LAYOUTS_CAPACITY];
    size_t layouts_count;

    void push_layout(Layout layout);
    Layout pop_layout();
    Layout *top_layout();

    void begin(V2<float> pos, float pad);
    void begin_layout(Layout::Kind kind, float pad);
    bool button(Renderer *renderer, Atlas *atlas, RGBA color, V2<float> size, Id id);
    bool screen(Id id);
    void end_layout();
    void end();
};

#endif // SOMETHING_UI_HPP_
