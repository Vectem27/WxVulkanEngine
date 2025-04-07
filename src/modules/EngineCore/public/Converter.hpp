#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include "Quaternion.hpp"
#include <string>

inline std::string ToString(Quaternion<float> quat)
{
    char buffer[50];
    sprintf(buffer, "Quat{x=%+.2f; y=%+.2f; z=%+.2f; w= %+.2f}", quat.x, quat.y, quat.z, quat.w);
    return buffer;
}

inline std::string ToString(Vector4f vec)
{
    char buffer[50];
    sprintf(buffer, "Vec4{x=%+.2f; y=%+.2f; z=%+.2f; w= %+.2f}", vec.x, vec.y, vec.z, vec.w);
    return buffer;
}

inline std::string ToString(Vector3f vec)
{
    char buffer[50];
    sprintf(buffer, "Vec3{x=%+.2f; y=%+.2f; z=%+.2f}", vec.x, vec.y, vec.z);
    return buffer;
}

inline std::string ToString(Vector2f vec)
{
    char buffer[50];
    sprintf(buffer, "Vec2{x=%+.2f; y=%+.2f}", vec.x, vec.y);
    return buffer;
}

inline std::string EulerToString(Quaternion<float> rotation, EulerRotationSequence ers = ERS_DEFAULT)
{
    Vector3f rot = rotation.ToEulerDegrees(ers);

    char buffer[50];
    sprintf(buffer, "Rot{pitch=%+.2f; rol=%+.2f; yaw=%+.2f}", rot.x, rot.y, rot.z);
    return buffer;
}

inline std::string ToString(Matrix4f mat)
{
    char buffer[50];
    sprintf(buffer, "Mat{%+.2f; %+.2f; %+.2f; %+.2f}\n   {%+.2f; %+.2f; %+.2f; %+.2f}\n   {%+.2f; %+.2f; %+.2f; %+.2f}\n   {%+.2f; %+.2f; %+.2f; %+.2f}", 
        mat[0][0],mat[1][0],mat[2][0],mat[3][0],
        mat[0][1],mat[1][1],mat[2][1],mat[3][1],
        mat[0][2],mat[1][2],mat[2][2],mat[3][2],
        mat[0][3],mat[1][3],mat[2][3],mat[3][3]);
    return buffer;
}

#endif // CONVERTER_HPP