#pragma once
#include "macros.hpp"
#include "colors.hpp"
#include "shaders.hpp"

enum __Vertex_Attributes {
    POSITION = 0,
    UV = 1,
    NORMAL = 2,
    COLOR = 3
};

struct Vertex {

    vec3 pos;
    vec2 uv;
    vec3 normal;

};

class Material {

private:

    Color Ka; // Ambient lighting
    Color Kd; // Diffuse lighting
    Color Ks; // Specular lighting
    float Ns; // Brightness
    float Ni; // Refraction coefficient
    float d;  // Opacity
    unsigned int texture_id;

public:

    Material();

    Material(
        Color Ka,
        Color Kd,
        Color Ks,
        float Ns, 
        float Ni,
        float d,
        const std::string& texture_path
    );

};

class Mesh {

private:

    unsigned int id;
    Material material;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

public:

    Mesh(Material material, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

};

class Model {

private:

    vec3 pos;
    std::vector<Mesh> meshes;
    Shader shader;
    quat rotation;

public:

    Model(const std::string& model_path);
    Model(std::vector<Mesh> meshes, Shader shader);
    ~Model();

    void render(mat4 MVP) const;
    void set_pos(vec3 new_pos);
    void set_rotation(quat new_rotation);
    void update_rotation(quat delta_rotation);

};