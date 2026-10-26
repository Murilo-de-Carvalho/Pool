#include "include/application.hpp"

// ============================================================
//  Application
// ============================================================

Application::Application(const std::string& window_name, const int width, const int height) {

    if (width < 1)
        ERROR("width must be positive")

    if (height < 1)
        ERROR("height must be positive")

    this->width = width;
    this->height = height;
    this->window_name = window_name;
    this->window = nullptr;

}

Application::~Application() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}


void Application::init() {

    if (!glfwInit())
        ERROR("Failed to initialize GLFW");

    // Tell GLFW which version we're using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW which profile we're using (core)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->window = glfwCreateWindow(this->width, this->height, this->window_name.c_str(), NULL, NULL);

    // Check if the GLFW window was created successfully
    if (!(this->window)) 
        ERROR("Failed to create GLFW window")

    // Introduce the window into the current context
    glfwMakeContextCurrent(this->window);

    // Check if OpenGL was loaded successfully
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        ERROR("Failed to load OpenGL")

    // Load GLAD to configure OpenGL
    gladLoadGL();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, this->width, this->height);

}


int Application::get_width() {
    return this->width;
}
int Application::get_height() {
    return this->height;
}



// ============================================================
//  Functions
// ============================================================

bool window_should_close(const Application& app) {
    return glfwWindowShouldClose(app.window);
}