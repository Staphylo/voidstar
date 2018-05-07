#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

#include <tdogl/Shader.h>
#include <tdogl/Program.h>

template<class K, class V>
using ordered_map = std::vector<std::pair<K,V>>;
using ProgramDict = std::map<std::string, std::shared_ptr<tdogl::Program>>;
using ShaderDict = std::map<std::string, tdogl::Shader>;

class ShaderManager {
public:
    ShaderManager() {}
    ~ShaderManager() {}

    bool loadFolder(const std::string& folder);
    bool loadShader(const std::string& file, GLenum shaderType = 0);
    bool reload();

    std::shared_ptr<tdogl::Program> getProgram(
                            std::vector<std::string>&& shaders);

protected:
    ShaderDict shaders_;
};

