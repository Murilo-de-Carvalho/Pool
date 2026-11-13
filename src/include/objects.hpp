#pragma once
#include "macros.hpp"
#include "render.hpp"

class Cue {

private:

    Model model;
    vec3 anchor;
    float theta;
    float phi;
    float distance;
    float minimum_distance;
    float maximum_distance;

    mat4 get_matrix() const;

public:

    Cue(
        Model model,
        vec3 cue_ball_pos,
        float distance,
        float minimum_distance,
        float maximum_distance
    );
    Cue();
    ~Cue();

    void rotate_vertical(float degrees);
    void rotate_horizontal(float degrees);

    void set_components(float distance, float theta, float phi);
    void set_distance(float new_distance);
    void update_distance(float delta_distance);

    void render(mat4 view_matrix, mat4 projection_matrix) const;

    inline float get_distance() const {
        return distance;
    }

};