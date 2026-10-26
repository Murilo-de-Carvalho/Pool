#pragma once
#include "macros.hpp"

class Orbital_Camera {

private:

    vec3 anchor;
    float distance;
    float minimun_distance;
    quat rotation;

public:

    Orbital_Camera(vec3 anchor, float distance, float minimum_distance);
    ~Orbital_Camera();

    void set_anchor(vec3 new_anchor);
    void update_distance(float delta_distance);

};