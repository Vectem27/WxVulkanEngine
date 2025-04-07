#include "VulkanProjectorLight.h"
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ProjectorLightData VulkanProjectorLight::GetProjectorLightData() const
{
    return data;
}

void VulkanProjectorLight::SetTransform(Transform transform)
{
    auto r = transform.rotation.ToEuler(ERS_ZYX);
    transform.rotation = Rotator::FromEuler(-r.x, -r.y, -r.z, ERS_XYZ);

    union Mat
    {
        Matrix4<float> mat{0};
        glm::mat4 glmMat;
    };

    Mat mat;
    mat.mat = transform.rotation.GetRotationMatrix();

    glm::mat4 rotationMatrix = mat.glmMat;
    glm::vec3 forward = glm::vec4(glm::vec3( 1, 0, 0), 1.0f) * rotationMatrix; // X+ avant
    glm::vec3 right   = glm::vec4(glm::vec3( 0, 1, 0), 1.0f) * rotationMatrix; // Y+ droite
    glm::vec3 up      = glm::vec4(glm::vec3( 0, 0, 1), 1.0f) * rotationMatrix; // Z+ haut
    glm::vec3 pos = {transform.position.x, transform.position.y, transform.position.z};

    mat.glmMat = glm::lookAt(pos, pos + forward, -up);

    Matrix4f view = mat.mat;

    mat.glmMat = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 100.f);

    Matrix4f proj = mat.mat;

    data.viewProj = proj * view;

}
