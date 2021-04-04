#include "./something_program.hpp"
#include "./something_game.hpp"

const Seconds DELTA_TIME_SECS = 1.0f / static_cast<Seconds>(SCREEN_FPS);
const Milliseconds DELTA_TIME_MS =
    static_cast<Milliseconds>(floorf(DELTA_TIME_SECS * 1000.0f));

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

AABB<float> compute_gl_viewport(int w, int h)
{
    const float w_width = static_cast<float>(w);
    const float w_height = static_cast<float>(h);
    const float s_width = static_cast<float>(SCREEN_WIDTH);
    const float s_height = static_cast<float>(SCREEN_HEIGHT);

    float n_width = w_width;
    float n_height = w_width * (s_height / s_width);

    if (n_height > w_height) {
        n_height = w_height;
        n_width = w_height * (s_width / s_height);
    }

    return AABB(
               V2(w_width * 0.5f - n_width * 0.5f,
                  w_height * 0.5f - n_height * 0.5f),
               V2(n_width, n_height));
}

// TODO(#4): hot-reloadable configuration
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        panic("SDL ERROR: ", SDL_GetError());
    }
    defer(SDL_Quit());

    SDL_Window * const window =
        SDL_CreateWindow(
            "Something 2 -- Electric Boogaloo",
            0, 0,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window == NULL) {
        panic("SDL ERROR: ", SDL_GetError());
    }
    defer(SDL_DestroyWindow(window));

    SDL_GL_CreateContext(window);

    if (GLEW_OK != glewInit()) {
        panic("Could not initialize GLEW!");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        int major;
        int minor;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
        // TODO(#5): GL compatibility code
        // We need to be able to identify what versions of GL are
        // available and always pick the best suited one
        println(stdout, "GL version ", major, ".", minor);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // NOTE: The game object could be too big to put on the stack.
    // So we are allocating it on the heap.
    Game *game = new Game{};
    defer(delete game);
    game->init(window);

    Renderer *renderer = new Renderer{};
    defer(delete renderer);
    renderer->init();

    while (!game->quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F5) {
                game->regular_program.reload();
                game->particle_program.reload();
            } else {
                game->handle_event(&event);
            }
        }

        game->update(DELTA_TIME_SECS);

        {
            // TODO(#6): don't recompute the GL viewport on every frame
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            const auto viewport = compute_gl_viewport(w, h).map(floorf).cast_to<GLint>();
            glViewport(viewport.pos.x, viewport.pos.y, viewport.size.x, viewport.size.y);
        }

        if (game->regular_program.failed || game->particle_program.failed) {
            glClearColor(FAILED_BACKGROUND_COLOR.r,
                         FAILED_BACKGROUND_COLOR.g,
                         FAILED_BACKGROUND_COLOR.b,
                         FAILED_BACKGROUND_COLOR.a);
        } else {
            glClearColor(BACKGROUND_COLOR.r,
                         BACKGROUND_COLOR.g,
                         BACKGROUND_COLOR.b,
                         BACKGROUND_COLOR.a);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        game->render(renderer);

        SDL_GL_SwapWindow(window);

        SDL_Delay(DELTA_TIME_MS);
    }

    SDL_Quit();

    return 0;
}
