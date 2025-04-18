/**
 * Quaternion.cpp v1.0.0 30/08/2023
 *
 * Copyright (c) 2023, Robert Eisele (raw.org)
 * Licensed under the MIT license.
 **/

#include "Quaternion.hpp"
#include <cmath>
#include "Converter.hpp"
#include <iostream>
/**
 * Assigns a quaternion to the current quaternion
 */
template <typename T>
Quaternion<T> &Quaternion<T>::operator=(const Quaternion<T> &q)
{
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
    return *this;
}

/**
 * Adds two quaternions Q1 and Q2
 */
template <typename T>
Quaternion<T> &Quaternion<T>::operator+=(const Quaternion<T> &q)
{
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

/**
 * Subtracts a quaternions Q2 from Q1
 */
template <typename T>
Quaternion<T> &Quaternion<T>::operator-=(const Quaternion<T> &q)
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
    return *this;
}

/**
 * Scales a quaternion by a scalar
 */
template <typename T>
Quaternion<T> &Quaternion<T>::operator*=(T scale)
{
    w *= scale;
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

/**
 * Calculates the Hamilton product of two quaternions
 */
template <typename T>
Quaternion<T> &Quaternion<T>::operator*=(const Quaternion<T> &q)
{
    T w1 = w;
    T x1 = x;
    T y1 = y;
    T z1 = z;

    T w2 = q.w;
    T x2 = q.x;
    T y2 = q.y;
    T z2 = q.z;

    w = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    x = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    y = w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2;
    z = w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2;
    return *this;
}

/**
 * Calculates the squared length/modulus/magnitude or the norm of a quaternion
 */
template <typename T>
T Quaternion<T>::NormSq() const
{
    return w * w + x * x + y * y + z * z;
}

/**
 * Calculates the conjugate of a quaternion
 */
template <typename T>
const Quaternion<T> Quaternion<T>::Conjugate() const
{
    return Quaternion<T>(-x, -y, -z, w);
}


/**
 * Rotates a vector according to the current quaternion, assumes |q|=1
 *
 * @link https://raw.org/proof/vector-rotation-using-quaternions/
 */ 
template <typename T>
Vector3<T> Quaternion<T>::Rotate(const Vector3<T>& vector)
{
    Vector3<T> res;
    // t = 2q x v
    T tx = 2. * (y * vector.z - z * vector.y);
    T ty = 2. * (z * vector.x - x * vector.z);
    T tz = 2. * (x * vector.y - y * vector.x);

    // v + w t + q x t
    res.x = vector.x + w * tx + y * tz - z * ty;
    res.y = vector.y + w * ty + z * tx - x * tz;
    res.z = vector.z + w * tz + x * ty - y * tx;
    return res;
}

template <typename T>
const Vector3<T> Quaternion<T>::ToEuler(EulerRotationSequence ers)
{
    const Quaternion<T>& q = *this;
    switch(ers)
    {
    case ERS_ZYX:
        return ThreeAxisRot( 2*(q.x*q.y + q.w*q.z),
                        q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                    -2*(q.x*q.z - q.w*q.y),
                        2*(q.y*q.z + q.w*q.x),
                        q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                        ers);
    case ERS_ZXY:
        return ThreeAxisRot( -2*(q.x*q.y - q.w*q.z),
                        q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                        2*(q.y*q.z + q.w*q.x),
                        -2*(q.x*q.z - q.w*q.y),
                        q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                        ers);
    case ERS_YXZ:
        return ThreeAxisRot( 2*(q.x*q.z + q.w*q.y),
                        q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                    -2*(q.y*q.z - q.w*q.x),
                        2*(q.x*q.y + q.w*q.z),
                        q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                        ers);
    case ERS_YZX:
        return ThreeAxisRot( -2*(q.x*q.z - q.w*q.y),
                        q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                        2*(q.x*q.y + q.w*q.z),
                        -2*(q.y*q.z - q.w*q.x),
                        q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                        ers);
    case ERS_XYZ:
        return ThreeAxisRot( -2*(q.y*q.z - q.w*q.x),
                    q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                    2*(q.x*q.z + q.w*q.y),
                    -2*(q.x*q.y - q.w*q.z),
                    q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                    ers);
    case ERS_XZY:
        return ThreeAxisRot( 2*(q.y*q.z + q.w*q.x),
                        q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                    -2*(q.x*q.y - q.w*q.z),
                        2*(q.x*q.z + q.w*q.y),
                        q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z, 
                        ers);
    default:
        throw std::runtime_error("Invalid euler rotation sequence");
        return Vector3<T>();
    }
}

template <typename T>
Matrix4<T> Quaternion<T>::GetRotationMatrix() const
{
    Matrix4<T> res;
    T xx = x * x, yy = y * y, zz = z * z;
    T xy = x * y, xz = x * z, yz = y * z;
    T wx = w * x, wy = w * y, wz = w * z;
    
    res[0][0] = 1.0 - 2.0 * (yy + zz);
    res[1][0] = 2.0 * (xy - wz);
    res[2][0] = 2.0 * (xz + wy);
    res[3][0] = 0.0;

    res[0][1] = 2.0 * (xy + wz);
    res[1][1] = 1.0 - 2.0 * (xx + zz);
    res[2][1] = 2.0 * (yz - wx);
    res[3][1] = 0.0;

    res[0][2] = 2.0 * (xz - wy);
    res[1][2] = 2.0 * (yz + wx);
    res[2][2] = 1.0 - 2.0 * (xx + yy);
    res[3][2] = 0.0;

    res[0][3] = 0.0;
    res[1][3] = 0.0;
    res[2][3] = 0.0;
    res[3][3] = 1.0;
    
    return res;
}

template <typename T>
const Quaternion<T> Quaternion<T>::FromEuler(T pitch, T roll, T yaw, EulerRotationSequence ers)
{
    T px = -roll * 0.5;
    T py = -pitch * 0.5;
    T pz = yaw * 0.5;  // Retiré le signe négatif pour plus de cohérence

    T cX = std::cos(px);
    T cY = std::cos(py);
    T cZ = std::cos(pz);

    T sX = std::sin(px);
    T sY = std::sin(py);
    T sZ = std::sin(pz);

    switch (ers)
    {
    case ERS_ZXY:
        // Z -> X -> Y (intrinsic: Y * X * Z)
        return Quaternion<T>(
            sX * cY * cZ - cX * sY * sZ,
            cX * sY * cZ + sX * cY * sZ,
            cX * cY * sZ + sX * sY * cZ,
            cX * cY * cZ - sX * sY * sZ);
    case ERS_XYZ:
        // X -> Y -> Z (intrinsic: Z * Y * X)
        return Quaternion<T>(
            sX * cY * cZ + cX * sY * sZ,
            cX * sY * cZ - sX * cY * sZ,
            cX * cY * sZ + sX * sY * cZ,
            cX * cY * cZ - sX * sY * sZ);
    case ERS_YXZ:
        // Y -> X -> Z (intrinsic: Z * X * Y)
        return Quaternion<T>(
            sX * cY * cZ + cX * sY * sZ,
            cX * sY * cZ - sX * cY * sZ,
            cX * cY * sZ - sX * sY * cZ,
            cX * cY * cZ + sX * sY * sZ);
    case ERS_ZYX:
        // Z -> Y -> X (intrinsic: X * Y * Z)
        return Quaternion<T>(
            sX * cY * cZ - cX * sY * sZ,
            cX * sY * cZ + sX * cY * sZ,
            cX * cY * sZ - sX * sY * cZ,
            cX * cY * cZ + sX * sY * sZ);
    case ERS_YZX:
        // Y -> Z -> X (intrinsic: X * Z * Y)
        return Quaternion<T>(
            sX * cY * cZ + cX * sY * sZ,
            cX * sY * cZ + sX * cY * sZ,
            cX * cY * sZ - sX * sY * cZ,
            cX * cY * cZ - sX * sY * sZ);
    case ERS_XZY:
        // X -> Z -> Y (intrinsic: Y * Z * X)
        return Quaternion<T>(
            sX * cY * cZ - cX * sY * sZ,
            cX * sY * cZ - sX * cY * sZ,
            cX * cY * sZ + sX * sY * cZ,
            cX * cY * cZ + sX * sY * sZ);
    default:
        throw std::runtime_error("Invalid euler rotation sequence");
    }
}

template <typename T>
const Quaternion<T> Quaternion<T>::FromEulerDegrees(T pitch, T roll, T yaw, EulerRotationSequence ers)
{
    return FromEuler(ToRadians(pitch),ToRadians(roll),ToRadians(yaw),ers);
}

/**
 * Creates quaternion by a rotation given as axis-angle orientation
 */
template <typename T>
const Quaternion<T> Quaternion<T>::FromAxisAngle(const Vector3<T>& axis, T angle)
{
    
    Quaternion<T> ret;

    T halfAngle = angle * 0.5;

    T sin_2 = std::sin(halfAngle);
    T cos_2 = std::cos(halfAngle);

    T sin_norm = sin_2 / std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

    ret.w = cos_2;
    ret.x = axis.x * sin_norm;
    ret.y = axis.y * sin_norm;
    ret.z = axis.z * sin_norm;

    return ret;
}

// Explicit instantiation for common types
template class Quaternion<float>;
template class Quaternion<double>;