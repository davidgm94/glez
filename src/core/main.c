#include "type.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "sys.h"
#include "file.h"

typedef int s_opengl_handle;
typedef unsigned u_opengl_handle;

s_opengl_handle compile_shader(const char* filename, GLenum shader_type)
{
    char shader_relative_path[256] = "src/shaders/";
    strcat(shader_relative_path, filename);
    char* shader_content = load_file(shader_relative_path);
    s_opengl_handle shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char *const *)&shader_content, NULL);
    glCompileShader(shader);
    s32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) {
        printf("Shader %s compiled successfully.\n", filename);
    } else {
        char info_log[512];
        glGetShaderInfoLog(shader, COUNT_OF(info_log), NULL, info_log);
        printf("Shader %s failed to compile:\n%s\n", filename, info_log);
        _exit(success);
    }
    return shader;
}

s_opengl_handle create_program(s_opengl_handle vertex_shader, s_opengl_handle fragment_shader)
{
    s_opengl_handle shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    s32 success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (success) {
        printf("Shader program linked successfully.\n");
    } else {
        char info_log[512];
        glGetProgramInfoLog(shader_program, COUNT_OF(info_log), NULL, info_log);
        printf("Shader program failed to be linked:\n%s\n", info_log);
        _exit(success);
    }

    return shader_program;
}

typedef struct {
    u32 width, height;
} window_dimension;


typedef struct {
    f32 position[3];
    f32 color[3];
} f32_pos_f32_color_vertex;

typedef struct {
    f32 position[3];
} pos_vertex;

typedef struct {
    f32 position[3];
    f32 tex_coord[2];
} pos_tex_vertex;

typedef struct{
    GLenum type;
    u8 count;
    u8 size;
    u8 offset;
    u8 padding[1];
} uniform_metadata;

void fill_vertex_attributes(uniform_metadata* element_metadata_arr, u32 element_count, size_t type_size)
{
    u8 offset = 0;
    for (int i = 0; i < element_count; i++)
    {
        uniform_metadata metadata_i = element_metadata_arr[i];
        //printf("arg0: %d, arg1: %d, type..., arg_penul: %d, arg_last: %d\n", i, metadata_i.count, metadata_i.count * metadata_i.size, )
        glVertexAttribPointer(i, metadata_i.count, metadata_i.type, GL_FALSE, type_size, (void*)(u64)offset);
        glEnableVertexAttribArray(i);
        offset += metadata_i.count * metadata_i.size;
    }
}

window_dimension w_dimension = {
        .width = 1024,
        .height = 576,
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    w_dimension.width = width;
    w_dimension.width = height;
}

void glfw_error_callback(s32 error, const char* desc)
{
    frame_logger("[GLFW ERROR %d] %s\n", error, desc);
}

void shader_set_bool(s_opengl_handle shader_program, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), (s32)value);
}

void shader_set_int(s_opengl_handle shader_program, const char* name, s32 value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}

void shader_set_float(s_opengl_handle shader_program, const char* name, f32 value)
{
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}

//void shader_set_mat4f(s_opengl_handle shader_program, const char* name, mat4f m)
//{
//    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*) &m);
//}
void shader_set_mat4(s_opengl_handle shader_program, const char* name, mat4 m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, (const GLfloat*) m);
}

u_opengl_handle load_texture(const char* filename, bool transparency)
{
    GLenum format = transparency ? GL_RGBA : GL_RGB;
    u_opengl_handle texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channel_count;
    stbi_set_flip_vertically_on_load(true);
    u8* data = stbi_load(filename, &width, &height, &channel_count, 0);
    assert(data);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Texture %s could not be loaded.\n", filename);
        _exit(-1);
    }
    stbi_image_free(data);

    return texture;
}

typedef struct {
    vec3 pos, front, up;
    f32 yaw;
    f32 pitch;
    f32 last_x, last_y;
    f32 fov;
    bool first_mouse;
} camera;

camera game_camera;

f32 delta_time = 0.0f;
f32 last_frame = 0.0f;

f32 player_x = 0.0f;
f32 player_y = 0.0f;
f32 player_z = 0.0f;

f32 floor_x = 0.0f;
f32 floor_y = 0.0f;
f32 floor_z = 0.0f;

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float camera_speed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_muladds(game_camera.front, camera_speed, game_camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glm_vec3_muladds(game_camera.front, -camera_speed, game_camera.pos);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(game_camera.front, game_camera.up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_muladds(cross, -camera_speed, game_camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(game_camera.front, game_camera.up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_muladds(cross, camera_speed, game_camera.pos);
    }
    float scale = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player_y += scale;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player_y -= scale;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        player_x -= scale;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        player_x += scale;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
        floor_y += scale;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        floor_y -= scale;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        floor_x -= scale;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        floor_x += scale;
    }
}

// Not used
void glfw_cursor_pos_callback(GLFWwindow* window, f64 x, f64 y)
{
    if (game_camera.first_mouse)
    {
        game_camera.last_x = x;
        game_camera.last_y = y;
        game_camera.first_mouse = false;
    }

    f32 x_offset = (f32)x - game_camera.last_x;
    f32 y_offset = game_camera.last_y - (f32)y;
    game_camera.last_x = x;
    game_camera.last_x = y;

    f32 yaw = game_camera.yaw;
    f32 pitch = game_camera.pitch;

    f32 sensitivity = 0.00005f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    f32 rad_yaw = glm_rad(yaw);
    f32 rad_pitch = glm_rad(pitch);
    f32 cos_rad_pitch = cosf(rad_pitch);
    f32 cos_rad_yaw = cosf(rad_yaw);
    f32 sin_rad_pitch = sinf(rad_pitch);
    f32 sin_rad_yaw = sinf(rad_yaw);
    vec3 front = {
            cos_rad_yaw * cos_rad_pitch,
            sin_rad_pitch,
            sin_rad_yaw * cos_rad_pitch,
    };

//    glm_vec3_normalize(front);
    glm_vec3_copy(front, game_camera.front);

    game_camera.yaw = yaw;
    game_camera.pitch = pitch;
}

void glfw_scroll_callback(GLFWwindow* window, f64 x_offset, f64 y_offset)
{
    f32 fov = game_camera.fov;
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= y_offset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
    game_camera.fov = fov;
}

int main(int argc, char* argv[])
{
//    game_camera.pos[0] = 0.0f;
//    game_camera.pos[1] = 0.0f;
//    game_camera.pos[2] = 3.0f;
    game_camera.pos[0] = 0.0f;
    game_camera.pos[1] = 20.0f;
    game_camera.pos[2] = 20.0f;

    game_camera.front[0] = 0.0f;
    game_camera.front[1] = -1.0f;
    game_camera.front[2] = -1.0f;

    game_camera.up[0] = 0.0f;
    game_camera.up[1] = 1.0f;
    game_camera.up[2] = 0.0f;

    game_camera.yaw = -90.0f;
    game_camera.pitch = 0.0f;
    game_camera.last_x = w_dimension.width / 2.0f;
    game_camera.last_y = w_dimension.height / 2.0f;
    game_camera.fov = 45.0f;
    game_camera.first_mouse = true;

    const char* w_title = "FirstGame";

    s32 glfw_init = glfwInit();
    if (!(glfw_init == GLFW_TRUE)) {
        const char glfw_init_failed[] = "GLFW initialization failed! Exiting...\n";
        LOG_ERROR_AND_EXIT(glfw_init_failed);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(w_dimension.width, w_dimension.height, w_title, NULL, NULL);
    if (!window) {
        const char window_err_msg[] = "GLFW window couldn't be created\n";
        LOG_ERROR_AND_EXIT(window_err_msg);
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(glfw_error_callback);
    //glfwSetKeyCallback(window, glfw_key_callback);
    //glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    s32 glad_init = gladLoadGL();
    if (!glad_init) {
        const char glad_err_msg[] = "GLAD initialization failed. Exiting...\n";
        LOG_ERROR_AND_EXIT(glad_err_msg);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    s_opengl_handle vertex_shader = compile_shader("coord_tex_vert.glsl", GL_VERTEX_SHADER);
    s_opengl_handle fragment_shader = compile_shader("coord_tex_frag.glsl", GL_FRAGMENT_SHADER);
    s_opengl_handle shader_program = create_program(vertex_shader, fragment_shader);

    s_opengl_handle floor_vertex_shader = compile_shader("floor_vert.glsl", GL_VERTEX_SHADER);
    s_opengl_handle floor_frag_shader = compile_shader("floor_frag.glsl", GL_FRAGMENT_SHADER);
    s_opengl_handle floor_shader = create_program(floor_vertex_shader, floor_frag_shader);

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cube_positions[] = {
            {0.0f,  0.0f,  0.0f},
            {2.0f,  5.0f, -15.0f},
            {1.5f, -2.2f, -2.5f},
            {3.8f, -2.0f, -12.3f},
            {2.4f, -0.4f, -3.5f},
            {1.7f,  3.0f, -7.5f},
            {1.3f, -2.0f, -2.5f},
            {1.5f,  2.0f, -2.5f},
            {1.5f,  0.2f, -1.5f},
            {1.3f,  1.0f, -1.5f},
    };

    pos_tex_vertex floor_vertices[6] = {
            [0] = {
                    .position = {
                            1.0f, 0.0f, 1.0f,
                    },
                    .tex_coord = {
                            1.0f, 1.0f,
                    },
            },
            [1] = {
                    .position = {
                            -1.0f, 0.0f, 1.0f,
                    },
                    .tex_coord = {
                            0.0f, 1.0f,
                    },
            },
            [2] = {
                    .position = {
                            1.0f, 0.0f, -1.0f,
                    },
                    .tex_coord = {
                            1.0f, 0.0f,
                    },
            },
            [3] = {
                    .position = {
                            -1.0f, 0.0f, -1.0f,
                    },
                    .tex_coord = {
                            0.0f, 0.0f,
                    },
            },
            [4] = {
                    .position = {
                            -1.0f, 0.0f, 1.0f,
                    },
                    .tex_coord = {
                            0.0f, 1.0f,
                    },
            },
            [5] = {
                    .position = {
                            1.0f, 0.0f, -1.0f,
                    },
                    .tex_coord = {
                            1.0f, 0.0f,
                    },
            },
    };

    u_opengl_handle vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uniform_metadata metadata[2] = {
            [0] = {
                    .type = GL_FLOAT,
                    .count = 3,
                    .size = sizeof(f32),
                    .offset = my_offsetof(pos_tex_vertex, position)
            },
            [1] = {
                     .type = GL_FLOAT,
                     .count = 2,
                     .size = sizeof(f32),
                     .offset = my_offsetof(pos_tex_vertex, tex_coord),
            },
    };

    assert(sizeof(pos_tex_vertex) == sizeof(float) * 5);
    fill_vertex_attributes(metadata, COUNT_OF(metadata), sizeof(pos_tex_vertex));

    u_opengl_handle floor_vbo, floor_vao;
    glGenVertexArrays(1, &floor_vao);
    glGenBuffers(1, &floor_vbo);
    glBindVertexArray(floor_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

    fill_vertex_attributes(metadata, COUNT_OF(metadata), sizeof(pos_tex_vertex));

    u_opengl_handle texture1 = load_texture("assets/container.jpg", false);
    u_opengl_handle texture2 = load_texture("assets/awesomeface.png", true);
    u_opengl_handle floor_texture = load_texture("assets/floor.jpg", false);

    glUseProgram(shader_program);
    shader_set_int(shader_program, "texture1", 0);
    shader_set_int(shader_program, "texture2", 1);

    glUseProgram(floor_shader);
    shader_set_int(floor_shader, "tex", 0);

    while (!glfwWindowShouldClose(window))
    {
        frame_log_and_clear();
        BEGIN_TIME_BLOCK(TIME_FRAME_TOTAL);
        f32 _current_frame = glfwGetTime();
        delta_time = _current_frame - last_frame;
        last_frame = _current_frame;
        glfwPollEvents();
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        mat4 proj;
        glm_perspective(glm_rad(game_camera.fov), (f32)w_dimension.width / (f32)w_dimension.height, 0.1f, 100.0f, proj);
        vec3 camera_pos_plus_front;
        glm_vec3_add(game_camera.pos, game_camera.front, camera_pos_plus_front);
        glm_lookat(game_camera.pos, camera_pos_plus_front, game_camera.up, view);

        /// BEGIN GPU
        BEGIN_TIME_BLOCK(TIME_FRAME_GPU);
        glUseProgram(shader_program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(vao);
        shader_set_mat4(shader_program, "proj", proj);
        shader_set_mat4(shader_program, "view", view);

//        mat4 model = GLM_MAT4_IDENTITY;
//        glm_translate(model, (vec3){player_x, player_y, player_z});
//        shader_set_mat4(shader_program, "model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);

        for (u32 i = 0; i < COUNT_OF(cube_positions); i++)
        {
            mat4 model = GLM_MAT4_IDENTITY;
            glm_translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
            shader_set_mat4(shader_program, "model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glUseProgram(floor_shader);
        glBindVertexArray(floor_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture);
        mat4 floor_model = GLM_MAT4_IDENTITY;
        vec3 scale_v = {10.0f, 10.0f, 10.0f};
        glm_scale(floor_model, scale_v);
        glm_translate(floor_model, (vec3){floor_x, floor_y, floor_z });
        shader_set_mat4(floor_shader, "view", view);
        shader_set_mat4(floor_shader, "proj", proj);
        shader_set_mat4(floor_shader, "model", floor_model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        /// END GPU

        END_TIME_BLOCK(TIME_FRAME_GPU);
        END_TIME_BLOCK(TIME_FRAME_TOTAL);

        // TODO: this cripples performance when moving the window, use with care
        //  frame_set_window_title(window);
    }

    return 0;
}
