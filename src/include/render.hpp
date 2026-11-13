#pragma once
#include "macros.hpp"
#include "colors.hpp"
#include "shaders.hpp"

enum __Vertex_Attributes {
    POSITION = 0,
    NORMAL = 1,
    UV = 2,
    COLOR = 3
};

struct Vertex {

    vec3 pos;
    vec2 uv;
    vec3 normal;

};

class Material {

private:

    vec3 Ka; // Ambient lighting
    vec3 Ks; // Specular lighting
    float Ni; // Refraction coefficient
    float d;  // Opacity
    unsigned int Kd_map_id;
    unsigned int Ns_map_id;

    void get_maps(const std::string& Kd_map_path, const std::string& Ns_map_path);

public:

    Material();
    Material(
        vec3 Ka,
        vec3 Ks,
        float Ni,
        float d,
        const std::string& Kd_map_path,
        const std::string& Ns_map_path
    );

    ~Material();

    inline unsigned int get_Kd_map_id() const {
        return Kd_map_id;
    }

    inline unsigned int get_Ns_map_id() const {
        return Ns_map_id;
    }

};

class Mesh {

private:

    unsigned int id;
    Material material;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void setup_mesh();

public:

    Mesh(Material material, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    inline unsigned int get_id() const {
        return id;
    }

    inline unsigned int get_vertex_count() const {
        return vertices.size();
    }

    inline Material get_material() const {
        return material;
    }

};

class Model {

private:

    vec3 pos;
    std::vector<Mesh> meshes;
    Shader shader;
    quat rotation;

    std::vector<std::string> split_line(std::string line, std::string delimiter);

    void get_face_corner(
        const std::string& face_description,
        const std::vector<vec3>& v,
        const std::vector<vec3>& vn,
        const std::vector<vec2>& vt,
        std::vector<Vertex>& vertices
    );

    void read_face(
        const std::vector<std::string>& tokens,
        const std::vector<vec3>& v,
        const std::vector<vec3>& vn,
        const std::vector<vec2>& vt,
        std::vector<Vertex>& vertices
    );

    vec2 read_vec2(const std::vector<std::string>& tokens) const;
    vec3 read_vec3(const std::vector<std::string>& tokens) const;

    std::unordered_map<std::string, Material> parse_mtl(const std::filesystem::path& file_path);

    std::vector<Mesh> parse_obj(const std::filesystem::path& model_dir_name);

public:

    Model(const std::filesystem::path& model_dir_name, Shader shader);
    Model(std::vector<Mesh> meshes, Shader shader);
    Model();
    ~Model();

    void render(mat4 model, mat4 view, mat4 projection) const;
    void set_pos(vec3 new_pos);
    void set_rotation(quat new_rotation);
    void update_rotation(quat delta_rotation);

};