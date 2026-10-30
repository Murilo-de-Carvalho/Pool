#include "include/application.hpp"
#include "include/colors.hpp"
#include "include/macros.hpp"
#include "include/shaders.hpp"
#include "include/render.hpp"

int main() {

    Application app = Application("Window", 800, 800);

    app.init();

    set_background_color(FULL_BLACK);

    Model model = Model("balls");

    while (!window_should_close(app)) {

        glfwPollEvents();

        clear_background();

        glfwSwapBuffers(app.window);

    }

    return 0;
}