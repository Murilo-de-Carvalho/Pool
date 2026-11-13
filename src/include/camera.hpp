#pragma once
#include "macros.hpp"

class Orbital_Camera {

private:

    mat4 projection;
    vec3 anchor;
    float theta;
    float phi;
    float distance;
    float minimum_distance;
    float maximum_distance;

public:

    Orbital_Camera(
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
    );
    Orbital_Camera();

    ~Orbital_Camera();

    void rotate_vertical(float degrees);
    void rotate_horizontal(float degrees);

    void set_anchor(vec3 new_anchor);
    void update_distance(float delta_distance);

    mat4 get_projection_matrix() const;
    mat4 get_view_matrix() const;

    inline float get_distance() const {
        return distance;
    }

    inline float get_theta() const {
        return theta;
    }

    inline float get_phi() const {
        return phi;
    }

};
