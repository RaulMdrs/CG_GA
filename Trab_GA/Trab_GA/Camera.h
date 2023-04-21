#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        m_position = position;
        m_target = target;
        m_up = up;
    }

    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(m_position, m_target, m_up);
    }

    void SetPosition(glm::vec3 position) {
        m_position = position;
    }

    void SetTarget(glm::vec3 target) {
        m_target = target;
    }

    void SetUp(glm::vec3 up) {
        m_up = up;
    }

private:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
};