#include "include/colors.hpp"

// Black is the default color
Color::Color() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 255;
}

Color::Color(byte r, byte g, byte b, byte a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(unsigned int hex) {
    this->r =  hex >> 24;
    this->g =  hex >> 16 & 0x0000FFFF;
    this->b =  hex >>  8 & 0x000000FF;
    this->a =  hex       & 0x000000FF;
}

Color::~Color() {}



void set_background_color(Color color) {
    glClearColor(
        color.r/255.0f,
        color.g/255.0f,
        color.b/255.0f,
        color.a/255.0f
    );
}

void clear_background() {
    glClear(GL_COLOR_BUFFER_BIT);
}