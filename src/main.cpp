#include "include/application.hpp"
#include "include/colors.hpp"
#include "include/macros.hpp"
#include "include/shaders.hpp"
#include "include/render.hpp"

int main() {

    Application app = Application("Window", 800, 800);

    app.init();

    set_background_color(FULL_BLACK);

    Shader shader = Shader("./src/assets/shaders/default.vert", "./src/assets/shaders/default.frag");

    Model balls = Model("balls",shader);
    Model table = Model("table",shader);
    Model cue = Model("cue",shader);


    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)app.get_width()/(float)app.get_height(), 0.1f, 100.0f);


    vec3 camera_pos = vec3(1.0f, 0.0f, 0.0f);
    vec3 target = vec3(0.0f, 0.0f, 0.0f);

    vec3 direction = glm::normalize(camera_pos - target);

    vec3 up = vec3(0.0f, 0.0f, 1.0f);
    vec3 camera_right = glm::normalize(glm::cross(up, direction));

    vec3 _ = glm::cross(direction, camera_right);
    mat4 view;
    view = glm::lookAt(
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 0.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    );


    glm::mat4 model = glm::mat4(1.0f);


    while (!window_should_close(app)) {

        glfwPollEvents();

        clear_background();

        shader.bind();

        int model_loc = glGetUniformLocation(shader.get_id(), "u_model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

        int projection_loc = glGetUniformLocation(shader.get_id(), "u_projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(proj));

        int view_loc = glGetUniformLocation(shader.get_id(), "u_view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

        cue.render();

        glfwSwapBuffers(app.window);

    }

    return 0;
}