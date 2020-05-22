// Program options
#define GLM_DEBUG 0
#define FILE_DEBUGGING 0
#define DOUBLE_BUFFER 1
#define FASTER_SHADER_MATH 0
#define ALTERNATIVE_CAMERA 0

#if GLM_DEBUG
#include <cglm/cglm.h>
#endif
#include <glez/type.h>
#include <glez/log.h>
#include <glez/math.h>
#include <glez/sys.h>
#include <glez/file.h>
#include <glez/tex.h>
#include <glez/window.h>
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

void framebuffer_size_callback(GLFWwindow* window, s32 width, s32 height)
{
    glViewport(0, 0, width, height);
    window_dimension.width = width;
    window_dimension.height = height;
}

void glfw_error_callback(s32 error, const char* desc)
{
    frame_logger("[GLFW ERROR %d] %s\n", error, desc);
}

f32 delta_time = 0.0f;
f32 last_frame = 0.0f;

typedef struct {
    f32 posx, posy, posz;
    f32 rotx, roty, rotz;
    f32 scale;
} entity;

void entity_increase_position(entity* e, float dx, float dy, float dz)
{
    e->posx += dx;
    e->posy += dy;
    e->posz += dz;
}

void entity_increase_rotation(entity* e, float dx, float dy, float dz)
{
    e->rotx += dx;
    e->roty += dy;
    e->rotz += dz;
}

const f32 run_speed = 20;
const f32 turn_speed = 160;
typedef struct player {
    entity e;
    f32 current_speed;
    f32 current_turn_speed;

} player;

player mario = {};

mat4f create_transformation_matrix(vec3f translation, f32 rx, f32 ry, f32 rz, f32 s)
{
    mat4f T = MAT4_IDENTITY;
    T = translate(T, translation);
    T = rotate(T, rad(rx), VEC3(1.0f, 0.0f, 0.0f));
    T = rotate(T, rad(ry), VEC3(0.0f, 1.0f, 0.0f));
    T = rotate(T, rad(rz), VEC3(0.0f, 0.0f, 1.0f));
    // T = scale();
    T.r0 *= s;
    T.r1 *= s;
    T.r2 *= s;

    return T;
}

void init_player(player* p)
{
    p->e.posx = 0.0f;
    p->e.posy = 0.0f;
    p->e.posz = 0.0f;
    p->e.rotx = 0.0f;
    p->e.roty = 0.0f;
    p->e.rotz = 0.0f;
    p->e.scale = 0.05f;
}

void move_entity(entity* e, f32 current_turn_speed, f32 current_speed)
{
    entity_increase_rotation(e, 0.0f, current_turn_speed * delta_time, 0.0f);
    f32 distance = current_speed * delta_time;
    f32 dx = distance * sinf(rad(e->roty));
    f32 dz = distance * cosf(rad(e->roty));
    entity_increase_position(e, dx, 0.0f, dz);
}


f32 player_x = 0.0f;
f32 player_y = 0.0f;
f32 player_z = 0.0f;

f32 mario_front_x = 0.0f;
f32 mario_front_y = 1.0f;
f32 mario_front_z = 0.0f;

f32 player_rot_angle = 0.0f;

f32 floor_x = 0.0f;
f32 floor_y = 0.0f;
f32 floor_z = 0.0f;

typedef struct {
    vec3f position;
    vec3f front;
    vec3f up;
    f32 yaw;
    f32 pitch;
    f32 last_x, last_y;
} game_camera;

typedef struct quat_camera {
    quat rotation;
    vec3f position;
    f32 scale;
} quat_camera;

f32 fov = 45.0f;
bool first_mouse = true;
#if ALTERNATIVE_CAMERA
quat_camera camera;
#else
game_camera camera;
#endif

#if ALTERNATIVE_CAMERA
void init_camera(quat_camera* camera)
{
    camera->rotation = ? ? ? ? ;
    camera->position = VEC3(0.0f, 0.0f, 3.0f);
    camera->scale = ? ? ? ? ;
}
#else
void init_camera(game_camera* camera)
{
	camera->position = VEC3(0.0f, 0.0f, 3.0f);
	camera->front = VEC3(0.0f, 0.0f, -1.0f);
	camera->up = VEC3(0.01f, 1.0f, 0.0f);

	camera->yaw = -90.0f;
	camera->pitch = 0.0f;
	camera->last_x = 1024 / 2.0f;
	camera->last_y = 576 / 2.0f;
}
#endif

vec3f rotate_vector_quat(vec3f vector, quat q)
{
    vec3f result =
        (2.0f * vec3_dot(q, vector) * q) +
        ((q.w * q.w - vec3_dot(q, q)) * vector) +
        (2.0f * q.w * vec3_cross(q, vector));

    return result;
}
mat4f mat4_transpose(mat4f A)
{
    mat4f result;
    for (u32 i = 0; i < 4; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            result.row[i][j] = A.row[j][i];
        }
    }
    return result;
}

mat4f lookat_quat(quat rotation, vec3f position)
{
    mat4f view = {};
    // TODO: this rotation multiplication must take into account the quat-specific math
    vec3f v1 = vec3_normalize(rotation * VEC3(0.0f, 0.0f, 1.0f));
    vec3f v2 = vec3_cross(vec3_normalize(rotation * VEC3(0.0f, 1.0f, 0.0f)), v1);
    vec3f v3 = vec3_cross(v1, v2);
    
    view.row[0][0] = v2.x;
    view.row[0][1] = v3.x;
    view.row[0][2] = v1.x;

    view.row[1][0] = v2.y;
    view.row[1][1] = v3.y;
    view.row[1][2] = v1.y;

    view.row[2][0] = v2.z;
    view.row[2][1] = v3.z;
    view.row[2][2] = v1.z;

    view.row[3][0] = vec3_dot(-v2, position);
    view.row[3][1] = vec3_dot(-v3, position);
    view.row[3][2] = vec3_dot(-v1, position);

    return mat4_transpose(view);
}

void update_player_input(GLFWwindow* window)
{
    bool up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    bool right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    bool left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;

    if (up) {
        mario.current_speed = run_speed;
    } else if (down) {
        mario.current_speed = -run_speed;
    } if (!(up || down)) {
        mario.current_speed = 0.0f;
    }

    if (right) {
        mario.current_turn_speed = -turn_speed;
    } else if (left) {
        mario.current_turn_speed = turn_speed;
    } if (!(right || left)) {
        mario.current_turn_speed = 0.0f;
    }
}

void update_camera_input(GLFWwindow* window)
{
    f32 camera_speed = 2.5f * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.position += camera_speed * camera.front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position -= camera_speed * camera.front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.position -= vec3_normalize(vec3_cross(camera.front, camera.up)) * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.position += vec3_normalize(vec3_cross(camera.front, camera.up)) * camera_speed;
    }
}

void process_input(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);

    update_camera_input(window);
    update_player_input(window);
}

void glfw_scroll_callback(GLFWwindow* window, f64 x_offset, f64 y_offset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= 2.0f * y_offset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}

void glfw_cursor_callback(GLFWwindow* window, f64 x, f64 y)
{
    if (first_mouse) {
        camera.last_x = x;
        camera.last_y = y;
        first_mouse = false;
    }

    f32 x_offset = x - camera.last_x;
    f32 y_offset = camera.last_y - y;
    camera.last_x = x;
    camera.last_y = y;

    f32 sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    camera.yaw += x_offset;
    camera.pitch += y_offset;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    vec3f front;
    front.x = cosf(rad(camera.yaw)) * cosf(rad(camera.pitch));
    front.y = sinf(rad(camera.pitch));
    front.z = sinf(rad(camera.yaw)) * cosf(rad(camera.pitch));
    camera.front = vec3_normalize(front);
}

char buffer[10000];
void quick_debug(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    s32 bytes = stbsp_vsprintf(buffer, format, va);
    va_end(va);
    fwrite(buffer, bytes, 1, stdout);
}

void add_to_file_profiling(FILE* file, const char* fmt, ...)
{
    char buffer[4096];
    va_list va;
    va_start(va, fmt);
    s32 len = stbsp_vsprintf(buffer, fmt, va);
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
    s32 byte_ptr = stbsp_sprintf(buffer, "Matrix %s\n[\n", matrix_name);
    for (s32 i = 0; i < 4; i++)
    {
        byte_ptr += stbsp_sprintf(buffer + byte_ptr, "\t{");
        for (s32 j = 0; j < 4; j++)
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
    s32 byte_ptr = stbsp_sprintf(buffer, "Matrix %s\n[\n", matrix_name);
    for (s32 i = 0; i < 4; i++)
    {
        byte_ptr += stbsp_sprintf(buffer + byte_ptr, "\t{");
        for (s32 j = 0; j < 4; j++)
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

s32 main(s32 argc, char* argv[])
{
    init_camera(&camera);
    init_player(&mario);
    QueryPerformanceFrequency((LARGE_INTEGER*)&__game_performance_freq);
    mesh m = load_mesh("assets/mario/mario.obj");
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

    GLFWwindow* window = glfwCreateWindow(window_dimension.width, window_dimension.height, w_title, NULL, NULL);
    if (!window) {
        const char window_err_msg[] = "GLFW window couldn't be created\n";
        LOG_ERROR_AND_EXIT(window_err_msg);
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(glfw_error_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);
    glfwSetCursorPosCallback(window, glfw_cursor_callback);
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
            {
                    { 1.0f, 0.0f, 1.0f, },
                    { 1.0f, 1.0f, },
            },
            {
                    .position = { -1.0f, 0.0f, 1.0f, },
                    .tex_coord = { 0.0f, 1.0f, },
            },
            {
                    .position = { 1.0f, 0.0f, -1.0f, },
                    .tex_coord = { 1.0f, 0.0f, },
            },
            {
                    .position = { -1.0f, 0.0f, -1.0f, },
                    .tex_coord = { 0.0f, 0.0f, },
            },
            {
                    .position = { -1.0f, 0.0f, 1.0f, },
                    .tex_coord = { 0.0f, 1.0f, },
            },
            {
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
            {
                    .type = GL_FLOAT,
                    .count = 3,
                    .size = sizeof(f32),
            },
            {
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
        {
			.type = GL_FLOAT,
			.count = 4,
			.size = sizeof(f32),
        },
        {
			.type = GL_UNSIGNED_BYTE,
			.count = 4,
			.size = sizeof(u8),
        },
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

		process_input(window);

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
        glm_perspective(rad(game_camera.fov), (f32)window_dimension.width / (f32)window_dimension.height, 0.1f, 100.0f, proj);
        vec3 sum;
        glm_vec3_add(game_camera.pos, game_camera.front, sum);
        glm_lookat(game_camera.pos, sum, game_camera.up, view);
#else
        mat4f proj = perspective(rad(fov), (f32)window_dimension.width / (f32)window_dimension.height, 0.1f, 100.0f);
        mat4f view = lookat(camera.position, camera.position + camera.front, camera.up);
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
        move_entity(&mario.e, mario.current_turn_speed, mario.current_speed);
        mat4f mario_model = create_transformation_matrix(VEC3(mario.e.posx, mario.e.posy, mario.e.posz), mario.e.rotx, mario.e.roty, mario.e.rotz, mario.e.scale);
        //mat4f mario_model = MAT4_IDENTITY;
        //scale_v = VEC3(0.05f, 0.05f, 0.05f);
        //f32 mario_angle = rad(player_rot_angle);
        //mario_model = scale(mario_model, scale_v);
        //mario_model = translate(mario_model, VEC3(player_x, player_y, player_z));
        //mario_model = rotate(mario_model, mario_angle, VEC3(mario_front_x, mario_front_y, mario_front_z));
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
