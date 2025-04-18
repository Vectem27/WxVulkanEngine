#include "EngineCore.hpp"

Matrix4<float> Transform::GetTransformMatrix(bool ignoreScale) const
{
    Matrix4<float> scaleMat(1);
    if (!ignoreScale)
    {
        scaleMat[0][0] = scale.x;
        scaleMat[1][1] = scale.y;
        scaleMat[2][2] = scale.z;
        scaleMat[3][3] = 1.0;
    }

    Matrix4<float> transMat;
    transMat[3][0] = position.x;
    transMat[3][1] = position.y;
    transMat[3][2] = position.z;

    return (scaleMat * rotation.GetRotationMatrix()) * transMat;
}

Transform Transform::RelativeTo(Transform base)
{
    Transform result;
    result.position = base.position + base.rotation.Rotate(Vector3f(position.x * base.scale.x, position.y * base.scale.y ,position.z * base.scale.z));
    result.rotation = base.rotation * rotation;
    result.scale = Vector3f(scale.x * base.scale.x, scale.y * base.scale.y ,scale.z * base.scale.z);
    return result;
}
