#include "./something_game.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 60;

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

// TODO(#4): hot-reloadable configuration
int main()
{
    // NOTE: The game object could be too big to put on the stack.
    // So we are allocating it on the heap.
    Game *game = new Game{};
    defer(delete game);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        panic("SDL ERROR: ", SDL_GetError());
    }

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

    Renderer *renderer = new Renderer{};
    defer(delete renderer);
    renderer->init("./assets/textures/atlas.conf");

    game->keyboard = SDL_GetKeyboardState(NULL);

    while (!game->quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            game->handle_event(&event);
        }

        game->update(DELTA_TIME_SECS);

        {
            // TODO(#6): don't recompute the GL viewport on every frame
            int w, h;
            SDL_GetWindowSize(window, &w, &h);

            const float w_width = static_cast<float>(w);
            const float w_height = static_cast<float>(h);
            const float s_width = static_cast<float>(SCREEN_WIDTH);
            const float s_height = static_cast<float>(SCREEN_HEIGHT);

            float a_height = 0.0f;
            float a_width = 0.0f;

            if (w_width > w_height) {
                a_width = s_width * (w_height / s_height);
                a_height = w_height;
            } else {
                a_width = w_width;
                a_height = s_height * (w_width / s_width);
            }

            glViewport(
                static_cast<GLint>(floorf(w_width * 0.5f - a_width * 0.5f)),
                static_cast<GLint>(floorf(w_height * 0.5f - a_height * 0.5f)),
                static_cast<GLint>(a_width),
                static_cast<GLint>(a_height));
        }

        glClearColor(BACKGROUND_COLOR.r,
                     BACKGROUND_COLOR.g,
                     BACKGROUND_COLOR.b,
                     BACKGROUND_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT);

        game->render(renderer);

        renderer->present();

        SDL_GL_SwapWindow(window);

        SDL_Delay(DELTA_TIME_MS);
    }

    SDL_Quit();

    return 0;
}
