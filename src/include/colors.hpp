#pragma once
#include "macros.hpp"

struct Color {

    byte r;
    byte g;
    byte b;
    byte a;

    Color();
    Color(byte r, byte g, byte b, byte a);
    Color(unsigned int hex);

    ~Color();

};

void set_background_color(Color color);

// Just a wrapper
void clear_background();

const Color FULL_WHITE      = {255, 255, 255, 255};
const Color FULL_BLACK      = {  0,   0,   0, 255};

const Color FULL_RED        = {255,   0,   0, 255};
const Color FULL_GREEN      = {  0, 255,   0, 255};
const Color FULL_BLUE       = {  0,   0, 255, 255};

const Color FULL_MAGENTA    = {255,   0, 255, 255};
const Color FULL_CYAN       = {  0, 255, 255, 255};
const Color FULL_YELLOW     = {255, 255,   0, 255};



const Color RED        = {255,  44,  44, 255};
const Color GREEN      = {  0, 192,   0, 255};
const Color BLUE       = { 33, 150, 243, 255};