#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"

template<typename T>
struct Matrix4
{
    Matrix4(T value = 0, bool identity = true)
    {
        for(int i = 0; i < 4; ++i)
        {
            cols[i] = Vector4<T>(identity ? 0 : value);
            if(identity)
            {
                cols[i][i] = 1;
            }
        }
    }

    Vector4<T> cols[4];

    Vector4<T>& operator[](int index)
    {
        return cols[index];
    }

    const Vector4<T>& operator[](int index) const
    {
        return cols[index];
    }

    Matrix4<T> operator*(const Matrix4<T>& other) const
    {
        Matrix4<T> result;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result[i][j] = cols[i][0] * other[0][j] +
                               cols[i][1] * other[1][j] +
                               cols[i][2] * other[2][j] +
                               cols[i][3] * other[3][j];
            }
        }
        return result;
    }

    Vector4<T> operator*(const Vector4<T>& vec) const
    {
        Vector4<T> result;
        for (int i = 0; i < 4; ++i)
        {
            result[i] = cols[0][i] * vec[0] +
                        cols[1][i] * vec[1] +
                        cols[2][i] * vec[2] +
                        cols[3][i] * vec[3];
        }
        return result;
    }
};

using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;

#endif // MATRIX_HPP