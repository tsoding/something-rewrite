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

// TODO(#4): hot-reloadable configuration
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
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

    // TODO(#19): it's impossible to build an atlas with 0 margin
    // It triggers some asserts.
    game->atlas = Atlas::from_config("./assets/textures/atlas.conf", 10);

    Renderer *renderer = new Renderer{};
    defer(delete renderer);
    renderer->init();

    game->keyboard = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < 10; ++i) {
        game->tile_grid.get_tile(V2(i)).wall = true;
    }
    
    while (!game->quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F5) {
                renderer->reload_shaders();
            } else {
                game->handle_event(&event);
            }
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

        if (renderer->rect_program_failed) {
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

        renderer->present();

        if (!renderer->rect_program_failed) {
            glUniform2f(renderer->u_camera_position, game->camera.pos.x, game->camera.pos.y);
            glUniform1f(renderer->u_camera_z, game->camera.z);
            glUniform1f(renderer->u_time, static_cast<float>(SDL_GetTicks()) / 1000.0f);
        }

        SDL_GL_SwapWindow(window);

        SDL_Delay(DELTA_TIME_MS);
    }

    SDL_Quit();

    return 0;
}
