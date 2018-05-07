#include <Algo3DCube.hh>

bool
Algo3DCube::apply(GLfloat* vertices) {
    size_t pos = 0;
    for (size_t z = 0; z < depth_; ++z) {
        for (size_t y = 0; y < height_; ++y) {
            for (size_t x = 0; x < width_; ++x) {
                vertices[pos++] = ((float)x - (float)width_  / 2.0f) / 128;
                vertices[pos++] = ((float)y - (float)height_ / 2.0f) / 128;
                vertices[pos++] = ((float)z - (float)depth_  / 2.0f) / 128;
            }
        }
    }
    return true;
}
