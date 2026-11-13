#include "include/objects.hpp"

Cue::Cue(
    Model model,
    vec3 cue_ball_pos,
    float distance,
    float minimum_distance,
    float maximum_distance
) {

    this->model = model;
    anchor = cue_ball_pos;
    theta = CUE_INITIAL_THETA;
    phi = CUE_INITIAL_PHI;

    this->distance = distance;
    this->minimum_distance = minimum_distance;
    this->maximum_distance = maximum_distance;

}

Cue::Cue() {}

Cue::~Cue() {}

void Cue::rotate_vertical(float degrees) {
    if (phi + degrees > CUE_MAX_PHI) phi = CUE_MAX_PHI;
    else if (phi + degrees < CUE_MIN_PHI) phi = CUE_MIN_PHI;
    else phi += degrees;
}

void Cue::rotate_horizontal(float degrees) {
    theta += degrees;
}

mat4 Cue::get_matrix() const {

    vec3 pos = vec3(
        distance * cosf(glm::radians(theta)) * sinf(glm::radians(phi)),
        distance * sinf(glm::radians(theta)) * sinf(glm::radians(phi)),
        distance * cosf(glm::radians(phi))
    );

    vec3 diff = pos;

    pos += anchor;

    float x_rotation =  glm::atan(diff.z, sqrtf(diff.x * diff.x + diff.y * diff.y));
    float z_rotation = -glm::atan(diff.x, diff.y);

    mat4 matrix = mat4(1.0f);
    matrix = glm::translate(matrix, pos);
    matrix = glm::rotate(matrix, z_rotation, vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::rotate(matrix, x_rotation, vec3(1.0f, 0.0f, 0.0f));

    return matrix;
}

void Cue::set_components(float distance, float theta, float phi) {
    this->distance = distance;
    this->theta = theta;
    this->phi = phi;
}

void Cue::set_distance(float new_distance) {
    if (new_distance > maximum_distance) distance = maximum_distance;
    else if (new_distance < minimum_distance) distance = minimum_distance;
    else distance = new_distance;

}

void Cue::update_distance(float delta_distance) {
    if (distance + delta_distance > maximum_distance) distance = maximum_distance;
    else if (distance + delta_distance < minimum_distance) distance = minimum_distance;
    else distance += delta_distance;
}

void Cue::render(mat4 view_matrix, mat4 projection_matrix) const {
    model.render(get_matrix(), view_matrix, projection_matrix);
}