#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include "Matrix.hpp"

inline float ToRadians(float degrees) { return degrees * 3.14159265358979323846f / 180.0f; }
inline float ToDegree(float radians) { return radians * 180.0f / 3.14159265358979323846f; }

enum EulerRotationSequence
{
    ERS_ZYX,
    ERS_ZXY,
    ERS_XYZ,
    ERS_XZY,
    ERS_YXZ,
    ERS_YZX
};

#define ERS_DEFAULT ERS_ZYX

#define ERS_YPR ERS_ZYX
#define ERS_YRP ERS_ZXY
#define ERS_RPY ERS_XYZ
#define ERS_RYP ERS_XZY
#define ERS_PRY ERS_YXZ
#define ERS_PYR ERS_YZX

template <typename T>
struct Quaternion : public Vector4<T>
{
public:
    // Bring base class members into scope
    using Vector4<T>::x;
    using Vector4<T>::y;
    using Vector4<T>::z;
    using Vector4<T>::w;

    Quaternion() { w = 1; }
    Quaternion(T x, T y, T z, T w) { this->x = x; this->y = y; this->z = z; this->w = w; }

    Quaternion &operator=(const Quaternion &rhs);
    Quaternion &operator+=(const Quaternion &q);
    Quaternion &operator-=(const Quaternion &q);
    Quaternion &operator*=(T scale);
    Quaternion &operator*=(const Quaternion &q);

    bool operator==(const Quaternion<T> other) const
    {
        return Vector4<T>(other) == Vector4<T>(*this);
    }

    const Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }
    const Quaternion operator*(const Quaternion &q) const { return Quaternion(*this) *= q; }
    const Quaternion operator*(T scale) const { return Quaternion(w * scale, x * scale, y * scale, z * scale); }
    const Quaternion operator+(const Quaternion &q2) const
    {
        return Quaternion(w + q2.w, x + q2.x, y + q2.y, z + q2.z);
    }
    const Quaternion operator-(const Quaternion &q2) const
    {
        return Quaternion(w - q2.w, x - q2.x, y - q2.y, z - q2.z);
    }
    T NormSq() const;

    const Quaternion Conjugate() const;
    Vector3<T> Rotate(const Vector3<T>& vector);

    const Vector3<T> ToEuler(EulerRotationSequence ers = ERS_DEFAULT);
    const Vector3<T> ToEulerDegrees(EulerRotationSequence ers = ERS_DEFAULT)
    {
        Vector3<T> res = ToEuler(ers);
        return Vector3<T>(ToDegree(res.x), ToDegree(res.y), ToDegree(res.z));

    }
    Matrix4<T> GetRotationMatrix() const;

    static const Quaternion FromEuler(T pitch, T roll, T yaw, EulerRotationSequence ers = ERS_DEFAULT);
    static inline const Quaternion FromEulerDegrees(T pitch, T roll, T yaw, EulerRotationSequence ers = ERS_DEFAULT)
    {
        return FromEuler(ToRadians(pitch),ToRadians(roll),ToRadians(yaw),ers);
    }
    static const Quaternion FromAxisAngle(const Vector3<T>& axis, T angle);

private:
    Vector3<T> ThreeAxisRot(double r11, double r12, double r21, double r31, double r32, EulerRotationSequence ers)
    {
        T a = atan2(r31, r32);
        T b = asin(r21);
        T c = atan2(r11, r12);

        switch (ers)
        {
        case ERS_XYZ:
            return Vector3<T>(b, -c, a);
        case ERS_XZY: 
            return Vector3<T>(a, -c, b);
        case ERS_YXZ: 
            return Vector3<T>(c, -b, a);
        case ERS_YZX: 
            return Vector3<T>(c, -a, b);
        case ERS_ZXY: 
            return Vector3<T>(a, -b, c);
        case ERS_ZYX: 
            return Vector3<T>(b, -a, c);
        }     

        return Vector3<T>();
    }
};

#endif // QUATERNION_HPP