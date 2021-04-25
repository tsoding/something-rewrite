#ifndef SOMETHING_ENEMY_HPP_
#define SOMETHING_ENEMY_HPP_

struct Game;

struct Enemy {
    V2<float> pos;
    V2<float> vel;

    void render(const Game *game, Triangle_VAO *triangle_vao) const;
    void update(Game *game, Seconds dt);
};

#endif // SOMETHING_ENEMY_HPP_
