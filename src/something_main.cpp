#include "./something_program.hpp"
#include "./something_game.hpp"
#include "./config_loader.hpp"

const Seconds DELTA_TIME_SECS = 1.0f / static_cast<Seconds>(SCREEN_FPS);
const Milliseconds DELTA_TIME_MS =
    static_cast<Milliseconds>(floorf(DELTA_TIME_SECS * 1000.0f));

// inspired by https://github.com/Velho/ded/commit/44f9223d6ea7278243fa56c5a0c095bba0db4450
#if defined(__MINGW32__)  || defined(__MINGW64__)
#define CALLING_METHOD __stdcall
#else
#define CALLING_METHOD 
#endif

void CALLING_METHOD MessageCallback(GLenum source,
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

// TODO(#63): no sound system

const char *const VARS_CONF_PATH = "./assets/vars.conf";
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

    if (SDL_ShowCursor(0) < 0) {
        panic("SDL ERROR: ", SDL_GetError());
    }

    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        int major;
        int minor;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
        // TODO(#5): GL compatibility code
        // We need to be able to identify what versions of GL are
        // available and always pick the best suited one
        println(stdout, "GL version ", major, ".", minor);
    }

    SDL_GL_CreateContext(window);

    if (GLEW_OK != glewInit()) {
        panic("Could not initialize GLEW!");
    }

    if (!GLEW_ARB_draw_instanced) {
        println(stderr, "ARB_draw_instanced is not supported; game may not work properly!!\n");
    }

    if (!GLEW_ARB_instanced_arrays) {
        println(stderr, "ARB_instanced_arrays is not supported; game may not work properly!!\n");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    }

#ifndef SOMETHING_RELEASE
    reload_config_from_file(VARS_CONF_PATH);
#endif

    // NOTE: The game object could be too big to put on the stack.
    // So we are allocating it on the heap.
    Game *game = new Game{};
    defer(delete game);
    game->init(window);

    while (!game->quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
#ifndef SOMETHING_RELEASE
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F5) {
                game->renderer.reload();
                // TODO(#87): game does not indicate that vars.conf failed loading
                reload_config_from_file(VARS_CONF_PATH);
            }
#endif

            game->handle_event(&event);
        }

        game->update(DELTA_TIME_SECS);

        {
            // TODO(#6): don't recompute the GL viewport on every frame
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            const auto viewport = compute_gl_viewport(w, h).map(floorf).cast_to<GLint>();
            glViewport(viewport.pos.x, viewport.pos.y, viewport.size.x, viewport.size.y);
        }

        SDL_GL_SwapWindow(window);

        SDL_Delay(DELTA_TIME_MS);
    }

    SDL_Quit();

    return 0;
}
