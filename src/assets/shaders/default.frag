#version 330 core

// Variable that the gpu is gonna output
// It can be location 0, because ids in and out are differente (I think)
in vec2 uv;

out vec4 color_out;

uniform sampler2D u_texture; // Naming convention: u_{name} is always a uniform

void main() {
    //color_out = vertex_color;
    color_out = texture(u_texture, uv);
}