#include "include/camera.hpp"


Orbital_Camera::Orbital_Camera(
    vec3 anchor,
    float fov,
    float aspect_ratio,
    float near,
    float far,
    float theta,
    float phi,
    float distance,
    float minimum_distance,
    float maximum_distance
) {

    projection = glm::perspective(glm::radians(fov), aspect_ratio, near, far);

    /* mat4 view_matrix = glm::lookAt(
        vec3(distance, 0.0f, 0.0f),
        anchor,
        vec3(0.0f, 0.0f, 1.0f)
    ); */

    if (phi > CUE_MAX_PHI) phi = CUE_MAX_PHI;
    else if (phi < CUE_MIN_PHI) phi = CUE_MIN_PHI;

    this->anchor = anchor;
    this->theta = theta;
    this->phi = phi;
    this->distance = distance;
    this->minimum_distance = minimum_distance;
    this->maximum_distance = maximum_distance;

}

Orbital_Camera::Orbital_Camera() {

    projection = glm::perspective(glm::radians(45.0f), 1600.0f/900.0f, 0.1f, 100.0f);
    anchor = {0.0f, 0.0f, 0.0f};
    phi = 90.0f;
    theta = 0.0f;
    distance = 1.0f;
    minimum_distance = 0.1f;

}

Orbital_Camera::~Orbital_Camera() {}

void Orbital_Camera::rotate_vertical(float degrees) {
    if (phi + degrees > CUE_MAX_PHI) phi = CUE_MAX_PHI;
    else if (phi + degrees < CUE_MIN_PHI) phi = CUE_MIN_PHI;
    else phi += degrees;
}

void Orbital_Camera::rotate_horizontal(float degrees) {
    theta += degrees;
}

void Orbital_Camera::set_anchor(vec3 new_anchor) {
    anchor = new_anchor;
}

void Orbital_Camera::update_distance(float delta_distance) {
    if (distance + delta_distance < minimum_distance) return;
    if (distance + delta_distance > maximum_distance) return;
    else distance += delta_distance;
}

mat4 Orbital_Camera::get_projection_matrix() const {
    return projection;
}

mat4 Orbital_Camera::get_view_matrix() const {

    vec3 pos = vec3(
        distance * cosf(glm::radians(theta)) * sinf(glm::radians(phi)),
        distance * sinf(glm::radians(theta)) * sinf(glm::radians(phi)),
        distance * cosf(glm::radians(phi))
    );

    pos += anchor;

    return glm::lookAt(
        pos,
        anchor,
        vec3(0.0f, 0.0f, 1.0f)
    );

}