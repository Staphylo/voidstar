#include <iostream>

#include <platform.hpp>
#include <ShaderManager.hh>

bool
ShaderManager::reload()
{
    //for (auto& pair : shaders_) {
    //    pair.second = std::make_shared<tdogl::Program>(pair.first);
    //}
    return true;
}

bool
ShaderManager::loadFolder(const std::string& folder)
{
    // FIXME: use scandir
    std::vector<std::string> files = {
        "default.vert",
        "default.frag",
    };
    for (const auto& file : files) {
        if (!loadShader(file)) {
            return false;
        }
    }
    return true;
}

bool
ShaderManager::loadShader(const std::string& file, GLenum shaderType)
{


    if (shaderType != GL_VERTEX_SHADER || shaderType != GL_FRAGMENT_SHADER) {
        throw std::runtime_error("unknown shader type to load");
    }
    shaders_[file] = tdogl::Shader::shaderFromFile(
                                ResourcePath(file), shaderType);

    return true;
}

std::shared_ptr<tdogl::Program>
ShaderManager::getProgram(std::vector<std::string>&& shaders)
{
    std::vector<tdogl::Shader> vec;
    for (const auto& name : shaders) {
        vec.push_back(shaders_[name]);
    }
    auto* program = new tdogl::Program(vec);
    return std::make_shared<tdogl::Program>(program);
}
