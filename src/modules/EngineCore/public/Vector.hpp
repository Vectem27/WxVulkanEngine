#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>
#include <cmath>
#include <inttypes.h>

template<typename T>
struct Vector2
{
    union{struct{T x{0}, y{0};};};
    union{struct{T u, v;};};

    Vector2(T value = 0) : x(value), y(value) {}
    Vector2(T x, T y) : x(x), y(y) {}


    // Boolean operators
    bool operator==(const Vector2<T> other) const
    {
        return other.x == x &&
               other.y == y ;
    }

    bool operator!=(const Vector2<T> other) const { return !(*this == other); }



    //With self
    T& operator[](const int& index)
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            throw std::out_of_range("Invalid vector 2D index");
        }
    }

    Vector2 operator-()
    {
        Vector2 ret;
        ret.x = -x;
        ret.y = -y;
        return ret;
    }
    
    // With scalar
    template<typename U>
    Vector2 operator+(const U& scalar)
    {
        Vector2 ret;
        ret.x = x + scalar;
        ret.y = y + scalar;
        return ret;
    }

    template<typename U>
    Vector2 operator-(const U& scalar)
    {
        Vector2 ret;
        ret.x = x - scalar;
        ret.y = y - scalar;
        return ret;
    }

    template<typename U>
    Vector2 operator*(const U& scalar)
    {
        Vector2 ret;
        ret.x = x * scalar;
        ret.y = y * scalar;
        return ret;
    }

    template<typename U>
    Vector2 operator/(const U& scalar)
    {
        Vector2 ret;
        ret.x = x / scalar;
        ret.y = y / scalar;
        return ret;
    }

    // With other vector
    Vector2 operator+(const Vector2& other)
    {
        Vector2 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        return ret;
    }

    Vector2 operator-(const Vector2& other)
    {
        Vector2 ret;
        ret.x = x - other.x;
        ret.y = y - other.y;
        return ret;
    }

    T Dot(const Vector2& other) const
    {
        return x * other.x + y * other.y;
    }

    // Other operations
    double Norm() const
    {
        return sqrt(x * x + y * y);
    }

    double Dist(const Vector2& other) const
    {
        return (*this - other).Norm();
    }

    void Normalize()
    {
        *this = *this / Norm();
    }

    Vector2 GetNormalized() const
    {
        return *this / Norm();
    }
};

template<typename T>
struct Vector3
{
    union{struct{T x{0}, y{0}, z{0};};};

    Vector3(T value = 0) : x(value), y(value), z(value) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    // Boolean operators
    bool operator==(const Vector3<T> other) const
    {
        return other.x == x &&
               other.y == y &&
               other.z == z;
    }

    bool operator!=(const Vector3<T> other) const { return !(*this == other); }


    //With self
    T& operator[](const int& index)
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw std::out_of_range("Invalid vector 3D index");
        }
    }

    Vector3 operator-()
    {
        Vector3 ret;
        ret.x = -x;
        ret.y = -y;
        ret.z = -z;
        return ret;
    }
    
    // With scalar
    template<typename U>
    Vector3 operator+(const U& scalar)
    {
        Vector3 ret;
        ret.x = x + scalar;
        ret.y = y + scalar;
        ret.z = z + scalar;
        return ret;
    }

    template<typename U>
    Vector3 operator-(const U& scalar)
    {
        Vector3 ret;
        ret.x = x - scalar;
        ret.y = y - scalar;
        ret.z = z - scalar;
        return ret;
    }

    template<typename U>
    Vector3 operator*(const U& scalar)
    {
        Vector3 ret;
        ret.x = x * scalar;
        ret.y = y * scalar;
        ret.z = z * scalar;
        return ret;
    }

    template<typename U>
    Vector3 operator/(const U& scalar)
    {
        Vector3 ret;
        ret.x = x / scalar;
        ret.y = y / scalar;
        ret.z = z / scalar;
        return ret;
    }

    // With other vector
    Vector3 operator+(const Vector3& other)
    {
        Vector3 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        ret.z = z + other.z;
        return ret;
    }

    Vector3 operator-(const Vector3& other)
    {
        Vector3 ret;
        ret.x = x - other.x;
        ret.y = y - other.y;
        ret.z = z - other.z;
        return ret;
    }

    T Dot(const Vector3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3& other) const
    {
        Vector3 ret;
        ret.x = y * other.z - z * other.y;
        ret.y = z * other.x - x * other.z;
        ret.z = x * other.y - y * other.x;
        return ret;
    }

    T Triple(const Vector3& B, const Vector3& C) const
    {
        return Cross(B).Dot(C);
    }

    // Other operations
    double Norm() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    double Dist(const Vector3& other) const
    {
        return (*this - other).Norm();
    }

    void Normalize()
    {
        constexpr T zero{static_cast<T>(0)};
        const auto norm = Norm();
        if (norm != zero)
            *this = *this / norm;
    }

    Vector3 GetNormalized() const
    {
        constexpr T zero{static_cast<T>(0)};
        const auto norm = Norm();
        if (norm == zero)
            return Vector3(zero);
        return *this / norm;
    }
};

template<typename T>
struct Vector4
{
public:
    union{struct{T x{0}, y{0}, z{0}, w{0};};};

    Vector4(T value = 0) : x(value), y(value), z(value), w(value) {}
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}


    // Boolean operators
    bool operator==(const Vector4<T> other) const
    {
        return other.x == x &&
               other.y == y &&
               other.z == z &&
               other.w == w;
    }

    bool operator!=(const Vector4<T> other) const { return !(*this == other); }



    //With self
    T& operator[](const int& index)
    {
        switch (index)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: throw std::out_of_range("Invalid vector 4D index");
        }
    }

    const T& operator[](const int index) const
    {
        switch (index)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: throw std::out_of_range("Invalid vector 4D index");
        }
    }

    Vector4 operator-()
    {
        return Vector4(-x,-y,-z,-w);
    }
    
    // With scalar
    template<typename U>
    Vector4 operator+(const U& scalar)
    {
        Vector4 ret;
        ret.x = x + scalar;
        ret.y = y + scalar;
        ret.z = z + scalar;
        ret.w = w + scalar;
        return ret;
    }

    template<typename U>
    Vector4 operator-(const U& scalar)
    {
        Vector4 ret;
        ret.x = x - scalar;
        ret.y = y - scalar;
        ret.z = z - scalar;
        ret.w = w - scalar;
        return ret;
    }

    template<typename U>
    Vector4 operator*(const U& scalar)
    {
        Vector4 ret;
        ret.x = x * scalar;
        ret.y = y * scalar;
        ret.z = z * scalar;
        ret.w = w * scalar;
        return ret;
    }

    template<typename U>
    Vector4 operator/(const U& scalar)
    {
        Vector4 ret;
        ret.x = x / scalar;
        ret.y = y / scalar;
        ret.z = z / scalar;
        ret.w = w / scalar;
        return ret;
    }

    // With other vector
    Vector4 operator+(const Vector4& other)
    {
        Vector4 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        ret.z = z + other.z;
        ret.w = w + other.w;
        return ret;
    }

    Vector4 operator-(const Vector4& other)
    {
        Vector4 ret;
        ret.x = x - other.x;
        ret.y = y - other.y;
        ret.z = z - other.z;
        ret.w = w - other.w;
        return ret;
    }

    T Dot(const Vector4& other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // Other operations
    double Norm() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    double Dist(const Vector4& other) const
    {
        return (*this - other).Norm();
    }

    void Normalize()
    {
        *this = *this / Norm();
    }

    Vector4 GetNormalized() const
    {
        return *this / Norm();
    }
};

using Vector4d = Vector4<double>;
using Vector4f = Vector4<float>;
using Vector4i = Vector4<int32_t>;
using Vector3d = Vector3<double>;
using Vector3f = Vector3<float>;
using Vector3i = Vector3<int32_t>;
using Vector2d = Vector2<double>;
using Vector2f = Vector2<float>;
using Vector2i = Vector2<int32_t>;

#endif // VECTOR_HPP