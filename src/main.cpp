#include "include/application.hpp"
#include "include/colors.hpp"
#include "include/macros.hpp"
#include "include/shaders.hpp"
#include "include/render.hpp"
#include "include/camera.hpp"
#include "include/objects.hpp"

float last_x = 0.0f;
float last_y = 0.0f;

float last_frame = 0.0f;
float current_frame;
float dt;

bool should_rotate_view = false;
float x_offset_view;
float y_offset_view;

enum Game_State {
    DEFAULT,
    TAKING_SHOT,
    CUE_ANIMATION_FORWARDS,
    CUE_ANIMATION_BACKWARDS,
    FOLLOWING_CUEBALL
};

Game_State state = Game_State::DEFAULT;

Orbital_Camera camera = Orbital_Camera(
    CAM_INITIAL_ANCHOR,
    CAM_FOV,
    CAM_ASPECT_RATIO,
    CAM_NEAR,
    CAM_FAR,
    CAM_INITIAL_THETA,
    CAM_INITIAL_PHI,
    CAM_INITIAL_DISTANCE,
    CAM_MIN_DISTANCE,
    CAM_MAX_DISTANCE
);

Cue cue;

inline void update_dt() {
    current_frame = glfwGetTime();
    dt = current_frame - last_frame;
    last_frame = current_frame;
}

void key_callback(GLFWwindow* /* window */, int key, int /* scancode */, int action, int /* mods */) {

    if ( PRESSING_ESC ) {
        glfwTerminate();
        exit(0);
    }

    switch (state) {

        case Game_State::DEFAULT:           if ( PRESSING_SPACE )   state = Game_State::TAKING_SHOT;            break;
        case Game_State::TAKING_SHOT:       if ( RELEASING_SPACE )  state = Game_State::CUE_ANIMATION_FORWARDS; break;
        case Game_State::FOLLOWING_CUEBALL: 

            if ( PRESSING_W ) {
                state = Game_State::DEFAULT;
                cue.set_components(CUE_INITIAL_DISTANCE, camera.get_theta(), camera.get_phi());
            }

            break;

        default:
            break;

    }


    /* if ( PRESSING_SPACE && state == Game_State::DEFAULT ) {
        state = Game_State::TAKING_SHOT;
    }

    if ( RELEASING_SPACE && state == Game_State::TAKING_SHOT ) {
        taking_shot = false;
        cue_animation_forwards = true;
    } */

}

void mouse_callback(GLFWwindow* /* window */, double x_pos, double y_pos) {

    float x_offset = last_x - x_pos;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    //if (CAM_CANNOT_MOVE) return;

    x_offset *= MOUSE_SENSITIVITY;
    y_offset *= MOUSE_SENSITIVITY;

    camera.rotate_horizontal(x_offset);
    camera.rotate_vertical(y_offset);

    if (state == Game_State::FOLLOWING_CUEBALL) return;

    cue.rotate_horizontal(x_offset);
    cue.rotate_vertical(y_offset);

}

void scroll_callback(GLFWwindow*, double, double y_offset) {
    if (CAM_CANNOT_MOVE) return;
    camera.update_distance(-y_offset * SCROLL_SENSITIVITY);
}

int main() {

    Application app = Application(
        "Window",
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    app.init();

    glfwSetInputMode(app.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback(app.window, key_callback);
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetScrollCallback(app.window, scroll_callback);

    set_background_color(FULL_BLACK);

    Shader shader = Shader("./src/assets/shaders/default.vert", "./src/assets/shaders/default.frag");

    Model balls = Model("balls",shader);
    Model table = Model("table",shader);

    cue = Cue(
        Model("cue",shader),
        CUEBALL_INITIAL_POS,
        CUE_INITIAL_DISTANCE,
        CUE_MIN_DISTANCE,
        CUE_MAX_DISTANCE
    );

    //mat4 mat_cue;
    mat4 mat_table;
    mat4 mat_ball;

    mat4 cam_view;
    mat4 cam_proj;

    while (!window_should_close(app)) {

        update_dt();

        glfwPollEvents();

        clear_background();

        /* if (should_rotate_view) {
            
            should_rotate_view = false;
        } */

        //mat_cue = cue_obj.get_matrix();

        switch (state) {

            case Game_State::TAKING_SHOT: cue.update_distance(CUE_BACKWARDS_SPEED * dt); break;

            case Game_State::CUE_ANIMATION_FORWARDS:

                cue.update_distance(CUE_FORWARDS_SPEED * dt);

                if (cue.get_distance() == CUE_MIN_DISTANCE)
                    state = Game_State::FOLLOWING_CUEBALL;

                break;

            case Game_State::CUE_ANIMATION_BACKWARDS:

                cue.update_distance(CUE_BACKWARDS_SPEED * dt);

                if (cue.get_distance() >= CUE_INITIAL_DISTANCE) {
                    cue.set_distance(CUE_INITIAL_DISTANCE);
                    state = Game_State::DEFAULT;
                }

                break;

            default:
                break;

        }

        /* if (state == Game_State::TAKING_SHOT) {
            cue.update_distance(CUE_BACKWARDS_SPEED * dt);
        }

        else if (cue_animation_forwards) {

            cue.update_distance(CUE_FORWARDS_SPEED * dt);

            if (cue.get_distance() == CUE_MIN_DISTANCE) {
                cue_animation_forwards = false;
                cue_animation_backwards = true;
            }

        }

        else if (cue_animation_backwards) {

            cue.update_distance(CUE_BACKWARDS_SPEED * dt);

            if (cue.get_distance() >= CUE_INITIAL_DISTANCE) {
                cue.set_distance(CUE_INITIAL_DISTANCE);
                cue_animation_backwards = false;
            }
        } */

        mat_table = mat4(1.0f);
        mat_ball = mat4(1.0f);
        mat_ball = glm::translate(mat_ball, CUEBALL_INITIAL_POS);

        cam_view = camera.get_view_matrix();
        cam_proj = camera.get_projection_matrix();

        //if (state != Game_State::FOLLOWING_CUEBALL)
        cue.render(cam_view, cam_proj);
        balls.render(mat_ball, cam_view, cam_proj);
        table.render(mat_table, cam_view, cam_proj);

        glfwSwapBuffers(app.window);

    }

    return 0;
}