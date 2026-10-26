#pragma once
#include "macros.hpp"

class Application {

private:

    int width;
    int height;
    std::string window_name;


public:

    // It is used all the time, might as well be public
    GLFWwindow* window;

    Application(const std::string& window_name, const int width, const int height);
    ~Application();

    void init();

    int get_width();
    int get_height();

};

bool window_should_close(const Application& app);