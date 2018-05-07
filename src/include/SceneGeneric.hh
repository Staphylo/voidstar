#pragma once

#include <Manager.hh>
#include <Scene.hh>
#include <Algorithm.hh>

#include <tdogl/Program.h>

// SceneContext was actually a good idea to allow us to store a context per
// file and therefore resume in the exact same state as before
// Would contain:
//  - loader
//  - range
//  - algo
//  - rotation
//  - time
//  - frame
//  - buffers/indices? would probably be better to reapply

using VertIndices = std::vector<GLuint>;

/*struct SceneGenericContext {
    bool rotationEnabled;
    GLfloat degreeRotated;
    GLdouble time;
    GLuint frame;
    std::shared_ptr<Algorithm> algo;
    std::shared_ptr<tdogl::Program> program;
};*/

class SceneGeneric : public Scene {
public:
    SceneGeneric(Manager* manager);
    virtual ~SceneGeneric() { unload(); }

    virtual void init();
    virtual void clean();

    virtual void load(std::shared_ptr<Algorithm> algo);
    virtual void load(std::shared_ptr<Loader> loader,
                      std::shared_ptr<DataRange> range = 0);
    virtual void load(std::shared_ptr<tdogl::Program> program);
    virtual void unload();
    virtual void reload();
    virtual bool update(float elapsedTime);
    virtual void render();

protected:
    void draw();

    GLuint vao_;
    GLuint vbo_[2];
    VertIndices indices_;

    //std::shared_ptr<SceneGenericContext> ctx_;
    bool rotationEnabled_;
    GLfloat degreesRotated_;
    GLdouble time_;
    GLuint frame_;
    std::shared_ptr<tdogl::Program> program_;

    const size_t VBO_VERTICES = 0;
    const size_t VBO_COLORS = 1;
    const size_t VBO_SIZE = sizeof(vbo_) / sizeof(GLuint);
};
