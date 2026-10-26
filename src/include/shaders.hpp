#pragma once
#include "macros.hpp"

class Shader {

private:

    unsigned int id;

    std::string read_file(const std::string& path);

    unsigned int compile(unsigned int type, const std::string& src);

public:

    Shader();
    Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    ~Shader();

    void impromptu(const std::string& vert_src, const std::string& frag_src);

    inline unsigned int get_id() const {
        return id;
    }

    // Wrapper for glUseProgram()
    inline void bind() const {
        glUseProgram(id);
    }

    // Also a wrapper for glUseProgram()
    // Just binds a "nothing" shader
    inline void unbind() const {
        glUseProgram(0);
    }

};