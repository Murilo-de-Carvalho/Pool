#include "include/render.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"



// ============================================================
//  Material
// ============================================================

void Material::get_maps(const std::string& Kd_map_path, const std::string& Ns_map_path) {

    int width;
    int height;
    int BPP; // Bytes per pixel

    byte* buffer = NULL;

    glBindTexture(GL_TEXTURE_2D, 0);

    // OpenGL considers y=0 at the bottom while images understand y=0 at the top
    // That's why we're flipping vertically
    stbi_set_flip_vertically_on_load(true);
    buffer = stbi_load(Kd_map_path.c_str(), &width, &height, &BPP, 4);

    if (!buffer)
        ERROR("Failed to load texture")

    glGenTextures(1, &Kd_map_id);
    glBindTexture(GL_TEXTURE_2D, Kd_map_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (buffer)
        stbi_image_free(buffer);



    stbi_set_flip_vertically_on_load(true);
    buffer = stbi_load(Ns_map_path.c_str(), &width, &height, &BPP, 4);

    if (!buffer)
        ERROR("Failed to load texture")

    glGenTextures(1, &Ns_map_id);
    glBindTexture(GL_TEXTURE_2D, Ns_map_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (buffer)
        stbi_image_free(buffer);

}

Material::Material() {
    Ka = Ks = {0.0f, 0.0f, 0.0f};
    Ni = 0.0f;
    d = 0.0f;
    Kd_map_id = 0;
    Ns_map_id = 0;
}

Material::Material(
    vec3 Ka,
    vec3 Ks,
    float Ni,
    float d,
    const std::string& Kd_map_path,
    const std::string& Ns_map_path
) {

    this->Ka = Ka;
    this->Ks = Ks;

    this->Ni = Ni;

    if (d > 1.0f) d = 1.0f;
    else if (d < 0.0f) d = 0.0f;
    this->d = d;

    get_maps(Kd_map_path, Ns_map_path);

}

Material::~Material() {}



// ============================================================
//  Mesh
// ============================================================

struct __Mesh_Description {
    size_t v;
    size_t vn;
    size_t vt;
    size_t f;
};

void Mesh::setup_mesh() {

    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &id);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int), 
        &indices[0],
        GL_STATIC_DRAW
    );

    // vertex positions
    glEnableVertexAttribArray(__Vertex_Attributes::POSITION);	
    glVertexAttribPointer(__Vertex_Attributes::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    // vertex normals
    glEnableVertexAttribArray(__Vertex_Attributes::NORMAL);	
    glVertexAttribPointer(__Vertex_Attributes::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(__Vertex_Attributes::UV);
    glVertexAttribPointer(__Vertex_Attributes::UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    glBindVertexArray(0);

}

Mesh::Mesh(Material material, std::vector<Vertex> vertices, std::vector<unsigned int> indices) {

    this->material = material;
    this->vertices = vertices;
    this->indices = indices;

    setup_mesh();

}

Mesh::~Mesh() {}



// ============================================================
//  Model
// ============================================================



std::vector<std::string> Model::split_line(std::string line, std::string delimiter) {

    std::vector<std::string> split_line;

    size_t pos = 0;
    std::string token;

    // Finds token + delimiter, keeps token and erase both from the line until can't find delimiter
    while ( (pos = line.find(delimiter)) != std::string::npos ) {

        token = line.substr(0, pos);
        split_line.push_back(token);
        line.erase(0, pos + delimiter.size());

    }

    // Since it finds the delimiter AFTER the token, final token might not have a delimiter
    // So we append the rest of the line
    split_line.push_back(line);

    return split_line;
}



void Model::get_face_corner(
    const std::string& face_description,
    const std::vector<vec3>& v,
    const std::vector<vec3>& vn,
    const std::vector<vec2>& vt,
    std::vector<Vertex>& vertices
) {

    std::vector<std::string> v_vt_vn = split_line(face_description, "/");

    vec3 pos = v[std::stol(v_vt_vn[0]) - 1];
    vec2 tex = vt[std::stol(v_vt_vn[1]) - 1];
    vec3 normal = vn[std::stol(v_vt_vn[2]) - 1];

    vertices.push_back({
        pos,
        tex,
        normal
    });

}

void Model::read_face(
    const std::vector<std::string>& tokens,
    const std::vector<vec3>& v,
    const std::vector<vec3>& vn,
    const std::vector<vec2>& vt,
    std::vector<Vertex>& vertices
) {

    size_t tri_count = tokens.size() - 3;

    for (size_t i = 0; i < tri_count; i++) {
        get_face_corner(tokens[1], v, vn, vt, vertices);
        get_face_corner(tokens[2 + i], v, vn, vt, vertices);
        get_face_corner(tokens[3 + i], v, vn, vt, vertices);
    }

}



vec2 Model::read_vec2(const std::vector<std::string>& tokens) const {
    return {
        std::stof(tokens[1]),
        std::stof(tokens[2])
    };
}

vec3 Model::read_vec3(const std::vector<std::string>& tokens) const {
    return {
        std::stof(tokens[1]),
        std::stof(tokens[2]),
        std::stof(tokens[3])
    };
}



std::unordered_map<std::string, Material> Model::parse_mtl(const std::filesystem::path& file_path) {

    std::filesystem::path this_dir = file_path.parent_path(); // assets/model_dir_name
    std::unordered_map<std::string, Material> materials;

    std::vector<std::string> tokens;
    size_t material_count = 0;

    std::string line;
    std::ifstream file;

    file.open(file_path);

    for (byte i = 0; i < 2; i++) {
        // Removes "# Blender 5.2.2 LTS" and "# www.blender.org"
        std::getline(file, line);
    }

    std::getline(file, line); // Gets "# material_count"
    tokens = split_line(line, " ");
    material_count = std::stoul(tokens[2]);

    materials.reserve(material_count);

    for (size_t i = 0; i < material_count; i++) {

        std::string name;
        vec3 Ka;
        vec3 Ks;
        float Ni;
        float d;
        std::string Kd_map;
        std::string Ns_map;
        Kd_map = Ns_map = this_dir.string();
        Kd_map.append("/");
        Ns_map.append("/");

        std::getline(file, line); // Skips empty line

        std::getline(file, line); // newmtl name
        tokens = split_line(line, " ");
        name = tokens[1];

        std::getline(file, line); // Ka values
        tokens = split_line(line, " ");
        Ka = read_vec3(tokens);

        std::getline(file, line); // Ks values
        tokens = split_line(line, " ");
        Ks = read_vec3(tokens);

        std::getline(file, line); // Ni value
        tokens = split_line(line, " ");
        Ni = std::stof(tokens[1]);

        std::getline(file, line); // d value
        tokens = split_line(line, " ");
        d = std::stof(tokens[1]);

        std::getline(file, line); // Skips illum line

        std::getline(file, line); // Kd_map
        tokens = split_line(line, " ");
        Kd_map.append(tokens[1]);

        std::getline(file, line); // Ns_map value
        tokens = split_line(line, " ");
        Ns_map.append(tokens[1]);

        materials.insert({name, Material(Ka, Ks, Ni, d, Kd_map, Ns_map)});

    }
    file.close();

    return materials;
}

std::vector<Mesh> Model::parse_obj(const std::filesystem::path& model_dir_name) {


    std::filesystem::path dir = "./src/assets";
    dir /= model_dir_name; // dir = assets/model_dir_name

    std::filesystem::path files = dir;
    files /= model_dir_name; // files = assets/model_dir_name/model_dir_name

    std::filesystem::path obj_file = files;
    obj_file.concat(".obj"); // obj_file = assets/model_dir_name/model_dir_name.obj

    std::filesystem::path mtl_file = files;
    mtl_file.concat(".mtl"); // mtl_file = assets/model_dir_name/model_dir_name.mtl

    std::unordered_map<std::string, Material> materials = parse_mtl(mtl_file);

    /* size_t vertex_count = 0;
    size_t texture_count = 0;
    size_t normal_count = 0;
    size_t tri_count = 0; */
    std::vector<std::string> tokens;
    size_t mesh_count = 0;

    std::string line;
    std::ifstream file;

    file.open(obj_file);

    for (byte i = 0; i < 3; i++) {
        // Removes "# Blender 5.2.2 LTS", "# www.blender.org" and "mtllib name.mtl"
        std::getline(file, line);
    }

    std::getline(file, line); // Gets "# mesh_count"
    tokens = split_line(line, " ");

    mesh_count = std::stoul(tokens[2]);

    std::vector<__Mesh_Description> mesh_descriptions(mesh_count);
    size_t global_v_count = 0;
    size_t global_vn_count = 0;
    size_t global_vt_count = 0;
    //mesh_descriptions.reserve(mesh_count);
    
    std::getline(file, line); // Skips "o name"

    for (size_t i = 0; i < mesh_count; i++) {

        mesh_descriptions[i].v = 0;
        mesh_descriptions[i].vn = 0;
        mesh_descriptions[i].vt = 0;
        mesh_descriptions[i].f = 0;
        //mesh_descriptions[i].mat_name.reserve(1024);
        //mesh_descriptions[i].mat_name = "aaa";

        // v
        while (std::getline(file, line)) {

            tokens = split_line(line, " ");

            if ( tokens[0].compare("vn") == 0 ) {
                break;
            }

            ++mesh_descriptions[i].v;
            ++global_v_count;

        }

        // vn
        do {

            tokens = split_line(line, " ");

            if ( tokens[0].compare("vt") == 0 ) {
                break;
            }

            ++mesh_descriptions[i].vn;
            ++global_vn_count;

        } while (std::getline(file, line));

        // vt
        do {

            tokens = split_line(line, " ");

            if ( tokens[0].compare("s") == 0 ) {
                break;
            }

            ++mesh_descriptions[i].vt;
            ++global_vt_count;

        } while (std::getline(file, line));

        // Gets mat_name
        std::getline(file, line);
        tokens = split_line(line, " ");
        //printf("%s\n", line.c_str());
        //mesh_descriptions[i].mat_name = "aaa";// tokens[1];

        // f
        while (std::getline(file, line)) {

            tokens = split_line(line, " ");

            if ( tokens[0].compare("o") == 0 ) {
                break;
            }

            ++mesh_descriptions[i].f;

        }

    }
    file.close();


    // TODO: REMOVE
    /* for (size_t i = 0; i < mesh_count; i++) {
        printf("mesh %zu v count: %zu\n", i, mesh_descriptions[i].v);
        printf("mesh %zu vn count: %zu\n", i, mesh_descriptions[i].vn);
        printf("mesh %zu vt count: %zu\n", i, mesh_descriptions[i].vt);
        printf("mesh %zu f count: %zu\n\n", i, mesh_descriptions[i].f);
    } */

    /* std::vector<Mesh> m;
    return m; */

    file.open(obj_file);

    for (byte i = 0; i < 4; i++) {
        // Removes "# Blender 5.2.2 LTS", "# www.blender.org", "mtllib name.mtl" and "# mesh_count"
        std::getline(file, line);
    }

    std::vector<Mesh> meshes;
    meshes.reserve(mesh_count);

    std::vector<vec3> v(global_v_count);
    std::vector<vec3> vn(global_vn_count);
    std::vector<vec2> vt(global_vt_count);

    size_t v_seen = 0;
    size_t vn_seen = 0;
    size_t vt_seen = 0;

    for (size_t i = 0; i < mesh_count; i++) {

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        //vertices.reserve(mesh_descriptions[i].v);
        Material mat;

        std::getline(file, line); // Skips "o name"

        for (size_t j = 0; j < mesh_descriptions[i].v; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            v[v_seen++] = read_vec3(tokens);
        }

        for (size_t j = 0; j < mesh_descriptions[i].vn; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            vn[vn_seen++] = read_vec3(tokens);
        }

        for (size_t j = 0; j < mesh_descriptions[i].vt; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            vt[vt_seen++] = read_vec2(tokens);
        }

        std::getline(file, line); // Skips "s 1"
        // Getting material
        std::getline(file, line);
        tokens = split_line(line, " ");
        mat = materials[tokens[1]];

        for (size_t j = 0; j < mesh_descriptions[i].f; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            read_face(tokens, v, vn, vt, vertices);
        }

        indices.resize(vertices.size());
        std::iota(indices.begin(), indices.end(), 0);
        meshes.push_back(Mesh(mat, vertices, indices));

        /* v.clear();
        vn.clear();
        vt.clear();
        vertices.clear();
        indices.clear();

        v.resize(0);
        vn.resize(0);
        vt.resize(0);
        vertices.resize(0);
        indices.resize(0); */

        // TODO: REMOVE
        /* printf("size: %zu\n", vertices.size());
        for (Vertex v : vertices) {
            printf("pos: <%.2f, %.2f, %.2f>\n", v.pos.x, v.pos.y, v.pos.z);
            printf("normal: <%.2f, %.2f, %.2f>\n", v.normal.x, v.normal.y, v.normal.z);
            printf("uv: <%.2f, %.2f>\n\n", v.uv.x, v.uv.y);
        } */

    }
    file.close();

    // TODO: REMOVE
    /* printf("size: %zu\n", meshes.size());
    for (Mesh m : meshes) {
        printf("%p\n", (void*)&m);
    } */

    // TODO: REMOVE
    /* for (Vertex v : meshes[0].vertices) {
        printf("pos: <%.2f, %.2f, %.2f>\n", v.pos.x, v.pos.y, v.pos.z);
        printf("normal: <%.2f, %.2f, %.2f>\n", v.normal.x, v.normal.y, v.normal.z);
        printf("uv: <%.2f, %.2f>\n\n", v.uv.x, v.uv.y);
    } */
    

    return meshes;
}

Model::Model(const std::filesystem::path& model_dir_name, Shader shader) {
    this->meshes = parse_obj(model_dir_name);
    this->shader = shader;
}

Model::Model(std::vector<Mesh> /* meshes */, Shader /* shader */) {

}

Model::Model() {

}

Model::~Model() {

}



void Model::render(mat4 model, mat4 view, mat4 projection) const {

    shader.bind();

    int model_loc = glGetUniformLocation(shader.get_id(), "u_model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    int view_loc = glGetUniformLocation(shader.get_id(), "u_view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    int projection_loc = glGetUniformLocation(shader.get_id(), "u_projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    for (size_t i = 0; i < meshes.size(); i++) {

        Material mat = meshes[i].get_material();

        glBindTexture(GL_TEXTURE_2D, mat.get_Kd_map_id());
        glBindVertexArray(meshes[i].get_id());
        
        glDrawElements(GL_TRIANGLES, meshes[i].get_vertex_count(), GL_UNSIGNED_INT, (void*) 0);

    }

    shader.unbind();
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Model::set_pos(vec3 /* new_pos */) {

}

void Model::set_rotation(quat /* new_rotation */) {

}

void Model::update_rotation(quat /* delta_rotation */) {

}