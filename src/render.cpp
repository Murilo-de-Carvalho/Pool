#include "include/render.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"



// ============================================================
//  Material
// ============================================================

void Material::get_texture(const std::string& texture_path) {

    int width;
    int height;
    int BPP; // Bytes per pixel

    byte* buffer = NULL;

    glBindTexture(GL_TEXTURE_2D, 0);

    // OpenGL considers y=0 at the bottom while images understand y=0 at the top
    // That's why we're flipping vertically
    stbi_set_flip_vertically_on_load(true);
    buffer = stbi_load(texture_path.c_str(), &width, &height, &BPP, 4);

    if (!buffer)
        ERROR("Failed to load texture")

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

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
    Ka = Kd = Ks = {0.0f, 0.0f, 0.0f};
    Ns = 0.0f;
    Ni = 0.0f;
    d = 0.0f;
    texture_id = 0;
}

Material::Material(
    vec3 Ka,
    vec3 Kd,
    vec3 Ks,
    float Ns, 
    float Ni,
    float d,
    const std::string& texture_path
) {

    this->Ka = Ka;
    this->Kd = Kd;
    this->Ks = Ks;

    this->Ns = Ns;
    this->Ni = Ni;

    if (d > 1.0f) d = 1.0f;
    else if (d < 0.0f) d = 0.0f;
    this->d = d;

    get_texture(texture_path);

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



/* std::unordered_map<std::string, Material> Model::parse_mtl(const std::string& file_path) {

} */

std::vector<Mesh> Model::parse_obj(const std::filesystem::path& model_dir_name) {

    std::vector<std::string> tokens;
    std::filesystem::path dir = "./src/assets";
    dir /= model_dir_name; // dir = assets/model_dir_name

    std::filesystem::path files = dir;
    files /= model_dir_name; // files = assets/model_dir_name/model_dir_name

    std::filesystem::path obj_file = files;
    obj_file.concat(".obj"); // obj_file = assets/model_dir_name/model_dir_name.obj

    std::filesystem::path mtl_file = files;
    mtl_file.concat(".mtl"); // mtl_file = assets/model_dir_name/model_dir_name.mtl

    //std::unordered_map<std::string, Material> materials = parse_mtl(mtl_file);

    std::vector<vec3> v;
    std::vector<vec3> vn;
    std::vector<vec2> vt;
    std::vector<Vertex> vertices;

    /* size_t vertex_count = 0;
    size_t texture_count = 0;
    size_t normal_count = 0;
    size_t tri_count = 0; */
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

        }

        // vn
        do {

            tokens = split_line(line, " ");

            if ( tokens[0].compare("vt") == 0 ) {
                break;
            }

            ++mesh_descriptions[i].vn;

        } while (std::getline(file, line));

        // vt
        do {

            tokens = split_line(line, " ");

            if ( tokens[0].compare("s") == 0 ) {
                break;
            }

            ++mesh_descriptions[i].vt;

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

    for (size_t i = 0; i < mesh_count; i++) {

        std::vector<vec3> v(mesh_descriptions[i].v);
        std::vector<vec3> vn(mesh_descriptions[i].vn);
        std::vector<vec2> vt(mesh_descriptions[i].vt);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        //vertices.reserve(mesh_descriptions[i].v);
        Material mat;

        std::getline(file, line); // Skips "o name"

        for (size_t j = 0; j < mesh_descriptions[i].v; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            v[j] = read_vec3(tokens);
        }

        for (size_t j = 0; j < mesh_descriptions[i].vn; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            vn[j] = read_vec3(tokens);
        }

        for (size_t j = 0; j < mesh_descriptions[i].vt; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            vt[j] = read_vec2(tokens);
        }

        std::getline(file, line); // Skips "s 1"
        // Getting material
        std::getline(file, line);
        //tokens = split_line(line, " ");
        //mat = materials[tokens[1]];

        for (size_t j = 0; j < mesh_descriptions[i].f; j++) {
            std::getline(file, line);
            tokens = split_line(line, " ");
            read_face(tokens, v, vn, vt, vertices);
        }

        indices.resize(vertices.size());
        std::iota(indices.begin(), indices.end(), 0);
        meshes.push_back(Mesh(mat, vertices, indices));

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

    /* while (std::getline(file, line)) {

    }


    while (std::getline(file, line)) {

        tokens = split_line(line, " ");

        if ( tokens[0].compare("v") == 0 ) {
            ++vertex_count;
        }

        else if ( tokens[0].compare("vn") == 0 ) {
            ++normal_count;
        }

        else if ( tokens[0].compare("vt") == 0 ) {
            ++texture_count;
        }

        else if ( tokens[0].compare("f") == 0 ) {
            // Counting the 'f'if there is 4 tokens (3 coords) return 1 tri
            // If 5 (4 coords) returns 2 and so on
            tri_count += tokens.size() - 3;
        }

        else if ( tokens[0].compare("o") == 0 ) {
            ++mesh_count;
        }

    }
    file.close();

    v.reserve(vertex_count);
    vn.reserve(normal_count);
    vt.reserve(texture_count);

    //tri * 3 vertex, upper limit
    vertices.reserve(tri_count * 3);

    while (std::getline(file, line)) {

        tokens = split_line(line, " ");

        if ( !(tokens[0].compare("v")) ) {
            v.push_back(read_vec3(tokens));
        }

        else if ( !(tokens[0].compare("vn")) ) {
            vn.push_back(read_vec3(tokens));
        }

        else if ( !(tokens[0].compare("vt")) ) {
            vt.push_back(read_vec2(tokens));
        }

        else if ( !(tokens[0].compare("f")) ) {
            read_face(tokens, v, vn, vt, vertices);
        }

    }
    file.close(); */

}

Model::Model(const std::filesystem::path& model_dir_name) {
    this->meshes = parse_obj(model_dir_name);
}

Model::Model(std::vector<Mesh> /* meshes */, Shader /* shader */) {

}

Model::~Model() {

}



void Model::render(mat4 /* MVP */) const {

}

void Model::set_pos(vec3 /* new_pos */) {

}

void Model::set_rotation(quat /* new_rotation */) {

}

void Model::update_rotation(quat /* delta_rotation */) {

}