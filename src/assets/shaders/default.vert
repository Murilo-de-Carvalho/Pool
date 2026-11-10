#version 330 core

// It will eventually be casted as a vec4 for matrix multiplication, so we define it as so
// OpenGL knows the real size and it can manage it for us
// Position that we pass from cpu to gpu (in)
// Location is the id of the attribute
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 temp_uv;

mat4 translationMatrix = mat4(
    1.0, 0.0, 0.0, 0.0,  // Column 0 (X basis)
    0.0, 1.0, 0.0, 0.0,  // Column 1 (Y basis)
    0.0, 0.0, 1.0, 0.0,  // Column 2 (Z basis)
    0.0, 0.0, 0.0, 1.0  // Column 3 (Translation Position)
);
//layout(location = 1) in vec4 color_in;

//out vec4 vertex_color;
out vec2 uv;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
    uv = temp_uv;
    //vertex_color = color_in;
}