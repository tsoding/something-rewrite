#ifndef SOMETHING_ENEMY_HPP_
#define SOMETHING_ENEMY_HPP_

struct Game;

struct Enemy {
    Index<AABB_Body> body_index;

    enum State {
        Ded = 0,
        Alive
    };

    State state;

    float direction;

    void render(const Game *game, Triangle_VAO *triangle_vao) const;
    void update(Game *game, Seconds dt);

    void kill(Game *game);

    float dummy_idle_direction(const Game *game) const;
};

#endif // SOMETHING_ENEMY_HPP_
