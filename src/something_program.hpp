#ifndef SOMETHING_PROGRAM_HPP_
#define SOMETHING_PROGRAM_HPP_

struct Program {
    bool failed;

    const char *vert_shader_path;
    const char *frag_shader_path;

    GLuint program;

    GLuint u_atlas;
    GLuint u_resolution;
    GLuint u_time;
    GLuint u_camera_position;
    GLuint u_camera_scale;

    static Program load_from_shader_files(const char *vert_shader_path,
                                          const char *frag_shader_path);
    bool reload();
    void use();
};

#endif // SOMETHING_PROGRAM_HPP_
