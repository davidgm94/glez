// Program options
#define GLM_DEBUG 0
#define FILE_DEBUGGING 0
#define DOUBLE_BUFFER 1
#define FASTER_SHADER_MATH 0
#define ALTERNATIVE_CAMERA 1

#if GLM_DEBUG
#include <cglm/cglm.h>
#endif
#include <glez/type.h>
#include <glez/log.h>
#include <glez/math.h>
#include <glez/sys.h>
#include <glez/file.h>
#include <glez/tex.h>
#include <glez_gl/gl.h>
#include <GLFW/glfw3.h>
#include <glez/model.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#if _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#endif
#include <stb_sprintf.h>

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

f32 delta_time = 0.0f;
f32 last_frame = 0.0f;

f32 player_x = 0.0f;
f32 player_y = 0.0f;
f32 player_z = 0.0f;

vec3f player_front = { 0 };

f32 floor_x = 0.0f;
f32 floor_y = 0.0f;
f32 floor_z = 0.0f;

typedef struct {
#if GLM_DEBUG
    vec3 pos, front, up, right, world_up;
#else
    vec3f pos, front, up, right, world_up;
#endif
    f32 yaw;
    f32 pitch;
    f32 last_x, last_y;
    f32 dx, dy;
    f32 fov;
    f32 mov_speed;
    f32 turn_speed;
    bool first_mouse;
} camera;

typedef struct quat_camera {
    quat rotation;
    vec3f position;
    f32 scale;
} quat_camera;

#if FASTER_SHADER_MATH
quat_camera game_camera;
#else
camera game_camera;
#endif

#if ALTERNATIVE_CAMERA
void update(void)
{
    game_camera.front.x = cosf(rad(game_camera.yaw)) * cosf(rad(game_camera.pitch));
    game_camera.front.y = sinf(rad(game_camera.pitch));
    game_camera.front.z = sinf(rad(game_camera.yaw)) * cosf(rad(game_camera.pitch));
    game_camera.front = vec3_normalize(game_camera.front);

    game_camera.right = vec3_normalize(vec3_cross(game_camera.front, game_camera.world_up));
    game_camera.up = vec3_normalize(vec3_cross(game_camera.right, game_camera.world_up));
}

void init_camera(vec3f position, vec3f up, f32 yaw, f32 pitch, f32 move_speed, f32 turn_speed)
{
    game_camera.pos = position;
    game_camera.up = up;
    game_camera.yaw = yaw;
    game_camera.pitch = pitch;
    game_camera.mov_speed = move_speed;
    game_camera.turn_speed = turn_speed;
    game_camera.front = VEC3(0.0f, 0.0f, -1.0f);

    update();
}

void glfw_new_process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        game_camera.pos += game_camera.front * game_camera.mov_speed * delta_time;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        game_camera.pos -= game_camera.front * game_camera.mov_speed * delta_time;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        game_camera.pos -= game_camera.right * game_camera.mov_speed * delta_time;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        game_camera.pos += game_camera.right * game_camera.mov_speed * delta_time;
    }
}

mat4f get_view_matrix(void)
{
    return lookat(game_camera.pos, game_camera.pos + game_camera.front, game_camera.up);
}

void glfw_new_mouse_callback(GLFWwindow* window, f64 xPos, f64 yPos)
{
    f32 x = xPos; f32 y = yPos;

    if (game_camera.first_mouse)
    {
        game_camera.last_x = x;
        game_camera.last_y = y;
        game_camera.first_mouse = false;
    }

    f32 lastx = game_camera.last_x;
    f32 lasty = game_camera.last_y;

    f32 dx = x - lastx;
    f32 dy = lasty - y;
    game_camera.dx = dx;
    game_camera.dy = dy;

    game_camera.last_x = x;
    game_camera.last_y = y;

    frame_logger("x: %.6f, y: %.6f\n", dx, dy);
}

#else
void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

#if GLM_DEBUG
    float camera_speed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glm_vec3_muladds(game_camera.front, camera_speed, game_camera.pos);
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
#else
    f32 camera_speed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        game_camera.pos = vec3_muladds(game_camera.front, camera_speed, game_camera.pos);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		game_camera.pos = vec3_muladds(game_camera.front, -camera_speed, game_camera.pos);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3f cross = vec3_cross(game_camera.front, game_camera.up);
        cross = vec3_normalize(cross);
        game_camera.pos = vec3_muladds(cross, -camera_speed, game_camera.pos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3f cross = vec3_cross(game_camera.front, game_camera.up);
        cross = vec3_normalize(cross);
        game_camera.pos = vec3_muladds(cross, camera_speed, game_camera.pos);
    }
#endif

    f32 mario_speed = 250.0f * delta_time;
    f32 experimental_speed = 250.0f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        player_z -= mario_speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player_z += mario_speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        player_x -= mario_speed;
        player_front.z -= experimental_speed;
    }
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player_x += mario_speed;
        player_front.z += experimental_speed;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        floor_y += mario_speed;
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
        floor_y -= mario_speed;
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        floor_x -= mario_speed;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        floor_x += mario_speed;
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

    f32 rad_yaw = rad(yaw);
    f32 rad_pitch = rad(pitch);
    f32 cos_rad_pitch = cosf(rad_pitch);
    f32 cos_rad_yaw = cosf(rad_yaw);
    f32 sin_rad_pitch = sinf(rad_pitch);
    f32 sin_rad_yaw = sinf(rad_yaw);
#if GLM_DEBUG
    vec3 front = {
            cos_rad_yaw * cos_rad_pitch,
            sin_rad_pitch,
            sin_rad_yaw * cos_rad_pitch,
    };

    glm_vec3_normalize(front);
    glm_vec3_copy(front, game_camera.front);
#else
    vec3f front = {
            cos_rad_yaw * cos_rad_pitch,
            sin_rad_pitch,
            sin_rad_yaw * cos_rad_pitch,
    };

//    glm_vec3_normalize(front);
    game_camera.front = front;
#endif
    game_camera.yaw = yaw;
    game_camera.pitch = pitch;
}
#endif

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

char buffer[10000];
void quick_debug(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    stbsp_vsprintf(buffer, format, va);
    va_end(va);
    OutputDebugStringA(buffer);
}

void add_to_file_profiling(FILE* file, const char* fmt, ...)
{
    char buffer[4096];
    va_list va;
    va_start(va, fmt);
    int len = stbsp_vsprintf(buffer, fmt, va);
    va_end(va);
    fwrite(buffer, len, 1, file);
}

#if GLM_DEBUG
void write_matrix(mat4 m, FILE* file, const char* matrix_name)
#else
void write_matrix(mat4f m, FILE* file, const char* matrix_name)
#endif
{
    char buffer[2048];
    int byte_ptr = stbsp_sprintf(buffer, "Matrix %s\n[\n", matrix_name);
    for (int i = 0; i < 4; i++)
    {
        byte_ptr += stbsp_sprintf(buffer + byte_ptr, "\t{");
        for (int j = 0; j < 4; j++)
        {
#if GLM_DEBUG
            f32 n = m[i][j];
#else
            f32 n = m.row[i][j];
#endif
            byte_ptr += stbsp_sprintf(buffer + byte_ptr, " %f, ", n);
        }
        byte_ptr += stbsp_sprintf(buffer + byte_ptr, "}\n");
    }
    byte_ptr += stbsp_sprintf(buffer + byte_ptr, "]\n\n");
    fwrite(buffer, byte_ptr, 1, file);
}

mat4f get_rand_mat4f(void)
{
    mat4f r;
	for (s32 i = 0; i < 4; i++)
	{
		for (s32 j = 0; j < 4; j++)
		{
			r.row[i][j] = rand();
		}
	}
    return r;
}

void write_matrix_debug(mat4f m, const char* matrix_name)
{
    char buffer[2048];
    int byte_ptr = stbsp_sprintf(buffer, "Matrix %s\n[\n", matrix_name);
    for (int i = 0; i < 4; i++)
    {
        byte_ptr += stbsp_sprintf(buffer + byte_ptr, "\t{");
        for (int j = 0; j < 4; j++)
        {
#if GLM_DEBUG
            f32 n = m[i][j];
#else
            f32 n = m.row[i][j];
#endif
            byte_ptr += stbsp_sprintf(buffer + byte_ptr, " %f, ", n);
        }
        byte_ptr += stbsp_sprintf(buffer + byte_ptr, "}\n");
    }
    byte_ptr += stbsp_sprintf(buffer + byte_ptr, "]\n\n");
    OutputDebugStringA(buffer);
}

typedef struct {
    quat rotation;
    vec3f position;
    f32 scale;
} game_object_3d;

#if _WIN32
s32 WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR lpcmdline, s32 ncmdshow)
#elif __linux__
s32 main(int argc, char* argv[])
#endif
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&__game_performance_freq);
    mesh m = load_mesh("assets/mario/mario.obj");
//    game_camera.pos[0] = 0.0f;
//    game_camera.pos[1] = 0.0f;
//    game_camera.pos[2] = 3.0f;
//    game_camera.front[0] = 0.0f;
//    game_camera.front[1] = 0.0f;
//    game_camera.front[2] = -1.0f;

//   GAME_START

    memset(&game_camera, 0, sizeof(game_camera));
#if 0
    init_camera(VEC3(0.0f, 0.0f, 0.0f), VEC3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 5.0f, 1.0f);
#else
    game_camera.pos[0] = 0.0f;
    game_camera.pos[1] = 20.0f;
    game_camera.pos[2] = 20.0f;

    game_camera.front[0] = 0.0f;
    game_camera.front[1] = -1.0f;
    game_camera.front[2] = -1.0f;

#if 1
    game_camera.up[0] = 0.0f;
    game_camera.up[1] = 1.0f;
    game_camera.up[2] = 0.0f;

    game_camera.world_up = VEC3(0.0f, 1.0f, 0.0f);
#endif

    game_camera.yaw = -90.0f;
    game_camera.pitch = 0.0f;
    game_camera.right = VEC3(1.0f, 0.0f, 0.0f);
    game_camera.mov_speed = 10.0f;
    game_camera.turn_speed = 1.0f;



#endif
    game_camera.last_x = w_dimension.width / 2.0f;
    game_camera.last_y = w_dimension.height / 2.0f;
    game_camera.fov = 45.0f;
    game_camera.first_mouse = true;

    player_front = VEC3(0.0f, 0.0f, -1.0f);
    const char* w_title = "FirstGame";

    s32 glfw_init = glfwInit();
    if (!(glfw_init == GLFW_TRUE)) {
        const char glfw_init_failed[] = "GLFW initialization failed! Exiting...\n";
        LOG_ERROR_AND_EXIT(glfw_init_failed);
    }

    glfwWindowHint( GLFW_DOUBLEBUFFER, DOUBLE_BUFFER);
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
#if ALTERNATIVE_CAMERA
    glfwSetCursorPosCallback(window, glfw_new_mouse_callback);
#endif
    glfwSetScrollCallback(window, glfw_scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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

    s_opengl_handle mario_vertex_shader = compile_shader("mario_vert.glsl", GL_VERTEX_SHADER);
    s_opengl_handle mario_frag_shader = compile_shader("mario_frag.glsl", GL_FRAGMENT_SHADER);
    s_opengl_handle mario_shader = create_program(mario_vertex_shader, mario_frag_shader);

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

#if GLM_DEBUG
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
#else
    vec3f cube_positions[] = {
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
#endif

    pos_tex_vertex floor_vertices[6] = {
            [0] = {
                    .position = { 1.0f, 0.0f, 1.0f, },
                    .tex_coord = { 1.0f, 1.0f, },
            },
            [1] = {
                    .position = { -1.0f, 0.0f, 1.0f, },
                    .tex_coord = { 0.0f, 1.0f, },
            },
            [2] = {
                    .position = { 1.0f, 0.0f, -1.0f, },
                    .tex_coord = { 1.0f, 0.0f, },
            },
            [3] = {
                    .position = { -1.0f, 0.0f, -1.0f, },
                    .tex_coord = { 0.0f, 0.0f, },
            },
            [4] = {
                    .position = { -1.0f, 0.0f, 1.0f, },
                    .tex_coord = { 0.0f, 1.0f, },
            },
            [5] = {
                    .position = { 1.0f, 0.0f, -1.0f, },
                    .tex_coord = { 1.0f, 0.0f, },
            },
    };

    u_opengl_handle box_vbo, box_vao;
    glGenVertexArrays(1, &box_vao);
    glGenBuffers(1, &box_vbo);
    glBindVertexArray(box_vao);
    glBindBuffer(GL_ARRAY_BUFFER, box_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uniform_metadata metadata[2] = {
            [0] = {
                    .type = GL_FLOAT,
                    .count = 3,
                    .size = sizeof(f32),
            },
            [1] = {
                     .type = GL_FLOAT,
                     .count = 2,
                     .size = sizeof(f32),
            },
    };

    assert(sizeof(pos_tex_vertex) == sizeof(float) * 5);
    fill_vertex_attributes(metadata, COUNT_OF(metadata), sizeof(pos_tex_vertex));

    u_opengl_handle floor_vbo, floor_vao;
    glGenVertexArrays(1, &floor_vao);
    glGenBuffers(1, &floor_vbo);
    glBindVertexArray(floor_vao);
    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

    fill_vertex_attributes(metadata, COUNT_OF(metadata), sizeof(pos_tex_vertex));

    u_opengl_handle mario_vbo, mario_vao, mario_ibo;
    glGenVertexArrays(1, &mario_vao);
    glGenBuffers(1, &mario_vbo);
    glGenBuffers(1, &mario_ibo);
    glBindVertexArray(mario_vao);
    glBindBuffer(GL_ARRAY_BUFFER, mario_vbo);
    glBufferData(GL_ARRAY_BUFFER, m.vertex_count * sizeof(pos_norm_tex_vertex), m.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mario_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.index_count * sizeof(u32), &m.indices[0], GL_STATIC_DRAW);

    uniform_metadata mario_metadata[3] =
    {
        [0] =
        {
			.type = GL_FLOAT,
			.count = 4,
			.size = sizeof(f32),
        },
        [1] =
        {
			.type = GL_UNSIGNED_BYTE,
			.count = 4,
			.size = sizeof(u8),
        },
        [2] =
        {
            .type = GL_FLOAT,
            .count = 2,
            .size = sizeof(f32),
        },
    };
    fill_vertex_attributes(mario_metadata, COUNT_OF(mario_metadata), sizeof(pos_norm_tex_vertex));

#define STB_FLIP_VERTICALLY 1
    texture_info texture_1, texture_2, floor_texture_, mario_texture_;
    texture_1 = load_texture("assets/container.jpg", STB_FLIP_VERTICALLY);
    texture_2 = load_texture("assets/awesomeface.png", STB_FLIP_VERTICALLY);
    floor_texture_ = load_texture("assets/floor.jpg", STB_FLIP_VERTICALLY);
    mario_texture_ = load_texture("assets/mario/Mario_Albedo.png", STB_FLIP_VERTICALLY);
    u_opengl_handle texture1, texture2, floor_texture, mario_texture;
    texture1 = gl_gen_texture(texture_1, false);
    texture2 = gl_gen_texture(texture_2, true);
    floor_texture = gl_gen_texture(floor_texture_, false);
    mario_texture = gl_gen_texture(mario_texture_, true);
    free_texture(&texture_1);
    free_texture(&texture_2);
    free_texture(&floor_texture_);
    free_texture(&mario_texture_);

    glUseProgram(shader_program);
    shader_set_int(shader_program, "texture1", 0);
    shader_set_int(shader_program, "texture2", 1);

    glUseProgram(floor_shader);
    shader_set_int(floor_shader, "tex", 0);

    glUseProgram(mario_shader);
    shader_set_int(mario_shader, "tex", 0);
    
    mat4f a = get_rand_mat4f(), b = get_rand_mat4f();
    mat4f c = mat4f_mul(a, b);
    write_matrix_debug(c, "experiment");
    //_mm_insert_ps()

    while (!glfwWindowShouldClose(window))
    {
        frame_log_and_clear();
        BEGIN_TIME_BLOCK(TIME_FRAME_TOTAL);
        f32 _current_frame = glfwGetTime();
        delta_time = _current_frame - last_frame;
        last_frame = _current_frame;
		glfwPollEvents();
#if ALTERNATIVE_CAMERA
        glfw_new_process_input(window);
#else
		process_input(window);
#endif
#if FILE_DEBUGGING
#if GLM_DEBUG
        FILE* profile_frame_file = fopen("profile_glm.log", "w+");
#else
        FILE* profile_frame_file = fopen("profile_noglm.log", "w+");
#endif
        add_to_file_profiling(profile_frame_file, "PROFILING FRAME:\n\n");
#endif

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if GLM_DEBUG
        mat4 proj, view;
        glm_perspective(rad(game_camera.fov), (f32)w_dimension.width / (f32)w_dimension.height, 0.1f, 100.0f, proj);
        vec3 sum;
        glm_vec3_add(game_camera.pos, game_camera.front, sum);
        glm_lookat(game_camera.pos, sum, game_camera.up, view);
#else
        mat4f proj = perspective(rad(game_camera.fov), (f32)w_dimension.width / (f32)w_dimension.height, 0.1f, 100.0f);
        mat4f view = lookat(game_camera.pos, vec3_add(game_camera.pos, game_camera.front), game_camera.up);
#endif
#if FILE_DEBUGGING
        write_matrix(proj, profile_frame_file, "Projection matrix");
        write_matrix(view, profile_frame_file, "View matrix");
#endif

        /// BEGIN GPU
        BEGIN_TIME_BLOCK(TIME_FRAME_GPU);
        glUseProgram(shader_program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(box_vao);
        shader_set_mat4(shader_program, "proj", proj);
        shader_set_mat4(shader_program, "view", view);

        for (u32 i = 0; i < COUNT_OF(cube_positions); i++)
        {
#if GLM_DEBUG
            mat4 model = GLM_MAT4_IDENTITY;
            glm_translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            glm_rotate(model, rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
#else
            mat4f model = MAT4_IDENTITY;
            model = translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = rotate(model, rad(angle), (vec3f) {1.0f, 0.3f, 0.5f, 0.0f});
#endif
#if FILE_DEBUGGING
            char name[10];
            stbsp_sprintf(name, "Model %u", i);
            write_matrix(model, profile_frame_file, name);
#endif
            shader_set_mat4(shader_program, "model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glUseProgram(floor_shader);
        glBindVertexArray(floor_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture);
#if GLM_DEBUG
        mat4 floor_model = GLM_MAT4_IDENTITY;
        vec3 scale_v = {10.0f, 10.0f, 10.0f};
        glm_scale(floor_model, scale_v);
        glm_translate(floor_model, (vec3){floor_x, floor_y, floor_z });
#else
        mat4f floor_model = MAT4_IDENTITY;
        vec3f scale_v = {10.0f, 10.0f, 10.0f};
        floor_model = scale(floor_model, scale_v);
        floor_model = translate(floor_model, (vec3f){0.0, 0.0, 0.0 });
#endif
#if FILE_DEBUGGING
        write_matrix(floor_model, profile_frame_file, "Floor model");
#endif
        shader_set_mat4(floor_shader, "view", view);
        shader_set_mat4(floor_shader, "proj", proj);
        shader_set_mat4(floor_shader, "model", floor_model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // MARIO
        glUseProgram(mario_shader);
        glBindVertexArray(mario_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mario_ibo);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mario_texture);

#if GLM_DEBUG
        mat4 mario_model = GLM_MAT4_IDENTITY;
        vec3 scale_v2 = { 0.1f, 0.1f, 0.1f};
        glm_scale(mario_model, scale_v2);
        glm_translate(mario_model, (vec3){floor_x, floor_y, floor_z });
#else
#if FASTER_SHADER_MATH
        f32 scale = 0.05f;
#else
        mat4f mario_model = MAT4_IDENTITY;
        scale_v = VEC3(0.05f, 0.05f, 0.05f);
        float angle = player_x;
        f32 mario_angle = rad(angle);
        mario_model = rotate(mario_model, mario_angle, 5.0f * VEC3(0.0f, 1.0f, 0.0f));
        mario_model = scale(mario_model, scale_v);
        mario_model = translate(mario_model, (vec3f) { 0.0f, player_y, player_z });
#endif
#endif

        shader_set_mat4(mario_shader, "view", view);
        shader_set_mat4(mario_shader, "proj", proj);
        shader_set_mat4(mario_shader, "model", mario_model);
        glDrawElements(GL_TRIANGLES, m.index_count, GL_UNSIGNED_INT, NULL);

#if DOUBLE_BUFFER
        glfwSwapBuffers(window);
#else
        glFlush();
#endif
        /// END GPU
#if FILE_DEBUGGING
        fclose(profile_frame_file);
        exit(0);
#endif

        END_TIME_BLOCK(TIME_FRAME_GPU);
        END_TIME_BLOCK(TIME_FRAME_TOTAL);

        // TODO: this cripples performance when moving the window, use with care
        //  frame_set_window_title(window);
    }

    return 0;
}

typedef struct {
    mesh m_mesh;
    s_opengl_handle shader;
    u_opengl_handle vao, ibo, texture;
} drawable;
