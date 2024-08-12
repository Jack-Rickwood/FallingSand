#include "glm/glm.hpp"

class Math {
public:
    static glm::fvec2 project(glm::fvec2 v1, glm::fvec2 v2) {
        return glm::dot(v1, v2) / glm::dot(v2, v2) * v2;
    }
};