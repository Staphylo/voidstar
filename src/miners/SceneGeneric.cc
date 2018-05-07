
#include <SceneGeneric.hh>

SceneGeneric::SceneGeneric(Manager *manager)
    : Scene(manager, SCENE_GENERIC),
      rotationEnabled_(false),
      degreesRotated_(0.),
      time_(0.),
      frame_(0),
      program_(0)
{
}

void
SceneGeneric::init() {
    resize(manager_->args()->width, manager_->args()->height);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &vao_);
    glGenBuffers(VBO_SIZE, vbo_);
}

void
SceneGeneric::clean() {
    glDeleteBuffers(VBO_SIZE, vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void
SceneGeneric::load(std::shared_ptr<Algorithm> algo) {
    if (algo_) {
        std::cout << "replacing algorithm" << std::endl;
    }
    algo_ = algo;
}

void
SceneGeneric::load(std::shared_ptr<Loader> loader,
                   std::shared_ptr<DataRange> range) {
    if (!algo_) {
        throw std::runtime_error("no algorithm loaded");
    }
    algo_->use(loader, range);
}

void
SceneGeneric::load(std::shared_ptr<tdogl::Program> program) {
    if (program_) {
        std::cout << "replacing program" << std::endl;
    }
    program_ = program;
    // handle program
}

void
SceneGeneric::unload() {
}

void
SceneGeneric::reload() {
}

bool
SceneGeneric::update(float elapsedTime) {
    GLfloat degreesPerSecond = 10.0f;
    if (rotationEnabled_) {
        degreesRotated_ += elapsedTime * degreesPerSecond;
        while (degreesRotated_ > 360.0f)
            degreesRotated_ -= 360.0f;
    }

    auto events = manager_->getEvents();
    float moveSpeed = events->modShift() ? 0.5 : 2.0;
    if (events->keyDown('S'))
        camera_.offsetPosition(elapsedTime * moveSpeed * -camera_.forward());
    else if (events->keyDown('W'))
        camera_.offsetPosition(elapsedTime * moveSpeed * camera_.forward());
    if (events->keyDown('A'))
        camera_.offsetPosition(elapsedTime * moveSpeed * -camera_.right());
    else if(events->keyDown('D'))
        camera_.offsetPosition(elapsedTime * moveSpeed * camera_.right());
    if (events->keyDown('Z'))
        camera_.offsetPosition(elapsedTime * moveSpeed * -glm::vec3(0,1,0));
    else if (events->keyDown('X'))
        camera_.offsetPosition(elapsedTime * moveSpeed * glm::vec3(0,1,0));
    if (events->keyPressed(' '))
        rotationEnabled_ = !rotationEnabled_;

    auto mouse = manager_->getMouse();
    mouse->getCursorPos();
    camera_.offsetOrientation(mouse->sensitivity * mouse->y,
                              mouse->sensitivity * mouse->x);

    //reset the mouse, so it doesn't go out of the window
    mouse->setCursorPos(0, 0);
    mouse->scrollY = 0.0;
    return true;
}

void
SceneGeneric::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_POINTS, indices_.size(), GL_UNSIGNED_INT, indices_.data());
    glBindVertexArray(0);
}

void
SceneGeneric::render() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_->use();
    program_->setUniform("camera", camera_.matrix());
    program_->setUniform("model",
        glm::rotate(glm::mat4(), glm::radians(degreesRotated_),
                    glm::vec3(0, 1, 0)));
    program_->setUniform("iFrame", frame_);
    program_->setUniform("iTime", time_);

    draw();

    program_->stopUsing();
    frame_++;
}
