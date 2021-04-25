#ifndef SOMETHING_ENEMY_HPP_
#define SOMETHING_ENEMY_HPP_

struct Game;

struct Enemy {
    Index<AABB_Body> body_index;

    void render(const Game *game, Triangle_VAO *triangle_vao) const;
    void update(Game *game, Seconds dt);

    void kill();
};

#endif // SOMETHING_ENEMY_HPP_
