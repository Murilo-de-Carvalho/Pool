#include "include/shaders.hpp"

std::string Shader::read_file(const std::string& path) {

    std::ifstream file;
    std::stringstream ss;
    std::string line;

    file.open(path);

    if (file.is_open() == false)
    ERROR("Failed to open shader file")

    while (getline(file, line)) {
        ss << line << '\n';
    }

    return ss.str();

}

unsigned int Shader::compile(unsigned int type, const std::string& src) {

    unsigned int id = glCreateShader(type);

    const char* src_cstr = src.c_str();
    // Especify we're using only one source, passing the source and,
    //since is a null terminated string, no need to specify the length
    glShaderSource(id, 1, &src_cstr, nullptr);
    glCompileShader(id);

    // Verify if it compiled ok
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* msg = (char*) alloca(length);
        glGetShaderInfoLog(id, length, &length, msg);

        fprintf(stderr, "[ERROR] Failed to compile shader\n%s\n", msg);
        glDeleteShader(id);

        return 0;

    }

    return id;
}

Shader::Shader() {
    this->id = 0;
}

Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path) {

    std::string src_vs = read_file(vertex_shader_path);
    std::string src_fs = read_file(fragment_shader_path);

    unsigned int program_id = glCreateProgram();
    unsigned int vs_id = compile(GL_VERTEX_SHADER, src_vs);
    unsigned int fs_id = compile(GL_FRAGMENT_SHADER, src_fs);

    // Equivalent of linking .o files together after compilation
    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    // Equivalent of deleting .o files after linking final executable
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);

    this->id = program_id;

}

void Shader::impromptu(const std::string& vert_src, const std::string& frag_src) {

    if (this->id != 0)
        ERROR("Shader already compiled, impromptu only works on empty Shader instances")

    unsigned int program_id = glCreateProgram();
    unsigned int vs_id = compile(GL_VERTEX_SHADER, vert_src);
    unsigned int fs_id = compile(GL_FRAGMENT_SHADER, frag_src);

    // Equivalent of linking .o files together after compilation
    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    // Equivalent of deleting .o files after linking final executable
    glDeleteShader(vs_id);
    glDeleteShader(fs_id);

    this->id = program_id;

}

Shader::~Shader() {
    glDeleteProgram(id);
}
