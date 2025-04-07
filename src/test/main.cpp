#include "EngineCore.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

#define IfFailed(cond, msg) if (!(cond)) { printf("Error line:%d : ", __LINE__); printf(msg); printf("\n");}
#define IfFailedP(cond, msg, ...) if (!(cond)) { printf("Error line:%d : ", __LINE__); printf(msg, __VA_ARGS__); printf("\n");}

typedef Quaternion<float> Quat;

bool TestQuatEqApprox(Quat quat, float x, float y, float z, float w)
{
    char buffer[50];
    sprintf(buffer, "quat{x=%.2f;y=%.2f;z=%.2f;w=%.2f}", x, y, z, w);
    return std::string(buffer) == ToString(quat);
}

bool TestQuatEqApprox(Quat quat, Quat q)
{
    return TestQuatEqApprox(quat, q.x, q.y, q.z, q.w);
}

bool Compare(Vector3f a, Vector3f b)
{
    return ToString(a)==ToString(b);
}

void QuaternionTests()
{
    /* FromEuler */
    Quat Qxyz = Quat::FromEuler(0.5f, 1.2f, 0.75f, ERS_XYZ);
    Quat Qxzy = Quat::FromEuler(0.5f, 1.2f, 0.75f, ERS_XZY);
    Quat Qyxz = Quat::FromEuler(0.5f, 1.2f, 0.75f, ERS_YXZ);
    Quat Qyzx = Quat::FromEuler(0.5f, 1.2f, 0.75f, ERS_YZX);
    Quat Qzxy = Quat::FromEuler(0.5f, 1.2f, 0.75f, ERS_ZXY);
    Quat Qzyx = Quat::FromEuler(0.5f, 1.2f, 0.75f, ERS_ZYX);

    bool res = false;
    res = TestQuatEqApprox(Qxyz, 0.5838602, -0.0103822, 0.4228871, 0.69294);
    IfFailed(res, "Failed FromEuler() XYZ");

    res = TestQuatEqApprox(Qxzy, 0.4342809, -0.0103822, 0.4228871, 0.7952728);
    IfFailed(res, "Failed FromEuler() XZY");

    res = TestQuatEqApprox(Qyxz, 0.5838602, -0.0103822, 0.162913, 0.7952728);
    IfFailed(res, "Failed FromEuler() YXZ");

    res = TestQuatEqApprox(Qyzx, 0.5838602, 0.3903853, 0.162913, 0.69294);
    IfFailed(res, "Failed FromEuler() YZX");

    res = TestQuatEqApprox(Qzxy, 0.4342809, 0.3903853, 0.4228871, 0.69294);
    IfFailed(res, "Failed FromEuler() ZXY");

    res = TestQuatEqApprox(Qzyx, 0.4342809, 0.3903853, 0.162913, 0.7952728);
    IfFailed(res, "Failed FromEuler() ZYX");

    /* ToEuler */
    res = Compare(Qxyz.ToEuler(ERS_XYZ), Vector3f(0.5f, 1.2f, 0.75f));
    IfFailed(res, "Failed ToEuler() XYZ");
    res = Compare(Qxzy.ToEuler(ERS_XZY), Vector3f(0.5f, 1.2f, 0.75f));
    IfFailed(res, "Failed ToEuler() XZY");
    res = Compare(Qyxz.ToEuler(ERS_YXZ), Vector3f(0.5f, 1.2f, 0.75f));
    IfFailed(res, "Failed ToEuler() YXZ");
    res = Compare(Qyzx.ToEuler(ERS_YZX), Vector3f(0.5f, 1.2f, 0.75f));
    IfFailed(res, "Failed ToEuler() YZX");
    res = Compare(Qzxy.ToEuler(ERS_ZXY), Vector3f(0.5f, 1.2f, 0.75f));
    IfFailed(res, "Failed ToEuler() ZXY");
    res = Compare(Qzyx.ToEuler(ERS_ZYX), Vector3f(0.5f, 1.2f, 0.75f));
    IfFailed(res, "Failed ToEuler() ZYX");

    /* Axis Angle*/

    Quat axAngl;

    axAngl = Quat::FromAxisAngle({1,0,0}, 0.6);
    res = TestQuatEqApprox(axAngl, 0.2955202, 0, 0, 0.9553365);
    IfFailed(res, "Failed from axis angle ({1,0,0}, 0.6)");

    axAngl = Quat::FromAxisAngle({0,1,0}, 0.7);
    res = TestQuatEqApprox(axAngl, 0, 0.3428978, 0, 0.9393727);
    IfFailed(res, "Failed from axis angle ({0,1,0}, 0.7)");

    axAngl = Quat::FromAxisAngle({0,0,1}, 0.8);
    res = TestQuatEqApprox(axAngl, 0, 0, 0.3894183, 0.921061);
    IfFailed(res, "Failed from axis angle ({0,1,0}, 0.7)");

    axAngl = Quat::FromAxisAngle({0.44,0.87,0.22}, 1.2);
    res = TestQuatEqApprox(axAngl, 0.2485794, 0.4915094, 0.1242897, 0.8253356);
    IfFailed(res, "Failed from axis angle ({0.44,0.87,0.22}, 1.2)");

    /* Rotate */

    Quat rot = Quat::FromAxisAngle(Vector3f({0,0,1}), ToRadians(90));
    res = Compare(rot.Rotate({1,0,0}), Vector3f({0,1,0}));
    IfFailed(res, "Failed to rotate vector");
}

void TransformTest()
{
    Transform t1 {{4, 2, 1}, Quat::FromEuler(0,0,0), {1, 1, 1}};
    Transform t2 {{0, 1, 0}, Quat::FromEuler(0,0,0), {1, 3, 2}};
    bool res;
    res = Compare(t2.RelativeTo(t1).position, (Vector3f){4, 3, 1});
    IfFailed(res, "Failes transform relative to (t2 to t1)");

    Transform t3 {{4, 2, 1}, Quat::FromEuler(0,0,ToRadians(180)), {1, 1, 1}};
    res = Compare(t2.RelativeTo(t3).position, (Vector3f){4, 1, 1});
    IfFailed(res, "Failes transform relative to (t2 to t3)");    

    Transform t4 {{0,0,0}, Quat::FromEuler(ToRadians(45),0,ToRadians(20)), {1, 1, 1}};
    res = TestQuatEqApprox(t4.RelativeTo(t3).rotation, Quat::FromEuler(ToRadians(-45), 0, ToRadians(200)));
    IfFailed(res, "Failes transform relative to (t4 to t3)"); 

    Transform t5 {{0,0,0}, Quat(), {0.75, 0.2, 0.5}};
    res = Compare(t2.RelativeTo(t5).scale, Vector3f(0.75, 0.6, 1.0));
    IfFailed(res, "Failes transform relative to (t2 to t5)"); 

}

void ArrayTest()
{
    std::vector<int> vec;
    std::vector<int> vec2;
    Array<int> arr;

    IfFailed(arr.GetSize() == 0, "Failed");
    arr.Add(1);
    vec.push_back(1);
    IfFailed(arr.GetSize() == 1, "Failed");
    IfFailed(arr[0] == 1, "Failed")
    arr.Add(4);
    vec.push_back(4);
    IfFailed(arr.GetSize() == 2, "Failed");
    IfFailed(arr[1] == 4, "Failed")

    arr.Add(5);
    IfFailed(arr.GetSize() == 3, "Failed");
    IfFailed(arr[2] == 5, "Failed")

    arr.Insert(9, 2);
    IfFailed(arr.GetSize() == 4, "Failed");
    IfFailed(arr[2] == 9, "Failed")
    IfFailed(arr[3] == 5, "Failed")
    
    vec.push_back(9);
    vec.push_back(5);
    
    for(const auto& val : arr) vec2.push_back(val);
    IfFailed(vec2 == vec, "Failed");

    arr.Insert(150, 6, true);
    IfFailed(arr.GetSize() == 7, "Failed");
    IfFailed(arr[5] == 0, "Failed")
    IfFailed(arr[6] == 150, "Failed")

    vec.push_back(0);
    vec.push_back(0);
    vec.push_back(150);
    vec2.clear();
    for(const auto& val : arr) vec2.push_back(val);
    IfFailed(vec2 == vec, "Failed");

    vec.resize(4);
    arr.Resize(4);
    IfFailed(arr.GetSize() == 4, "Failed");

    vec2.clear();
    for(const auto& val : arr) vec2.push_back(val);
    IfFailed(vec2 == vec, "Failed");

    vec.pop_back();
    arr.RemoveIndex(3);
    IfFailed(arr.GetSize() == 3, "Failed");

    vec2.clear();
    for(const auto& val : arr) vec2.push_back(val);
    IfFailed(vec2 == vec, "Failed");

    arr.Add(vec[2]);
    arr.RemoveValues(vec[2]);
    IfFailed(arr.GetSize() == 2, "Failed");

    vec.pop_back();
    vec2.clear();
    for(const auto& val : arr) vec2.push_back(val);
    IfFailed(vec2 == vec, "Failed");

    arr.Clear();
    IfFailed(arr.GetSize() == 0, "Failed");
}

int main()
{
    QuaternionTests();
    TransformTest();
    ArrayTest();
    printf("Tests end");
    return 0;
}