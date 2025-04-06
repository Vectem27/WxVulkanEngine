#include "EngineCore.hpp"

#include <iostream>

int main()
{
    
    /*
    std::cout << "Eul pitch : " << rotZYX.pitch << " roll : " << rotZYX.roll << " yaw : " << rotZYX.yaw << std::endl;
    
    
    std::cout << "XYZ Quat x : " << rotXYZ.ToQuat().x << 
                     " y : " << rotXYZ.ToQuat().y << 
                     " z : " << rotXYZ.ToQuat().z << 
                     " w : " << rotXYZ.ToQuat().w << std::endl;

    std::cout << "XZY Quat x : " << rotXZY.ToQuat().x << 
                     " y : " << rotXZY.ToQuat().y << 
                     " z : " << rotXZY.ToQuat().z << 
                     " w : " << rotXZY.ToQuat().w << std::endl;
                
    std::cout << "YZX Quat x : " << rotYZX.ToQuat().x << 
                     " y : " << rotYZX.ToQuat().y << 
                     " z : " << rotYZX.ToQuat().z << 
                     " w : " << rotYZX.ToQuat().w << std::endl;
                     
    std::cout << "YXZ Quat x : " << rotYXZ.ToQuat().x << 
                     " y : " << rotYXZ.ToQuat().y << 
                     " z : " << rotYXZ.ToQuat().z << 
                     " w : " << rotYXZ.ToQuat().w << std::endl;

    std::cout << "ZXY Quat x : " << rotZXY.ToQuat().x << 
                     " y : " << rotZXY.ToQuat().y << 
                     " z : " << rotZXY.ToQuat().z << 
                     " w : " << rotZXY.ToQuat().w << std::endl;
                     
    std::cout << "ZYX Quat x : " << rotZYX.ToQuat().x << 
                     " y : " << rotZYX.ToQuat().y << 
                     " z : " << rotZYX.ToQuat().z << 
                     " w : " << rotZYX.ToQuat().w << std::endl;

    return 0;*/

    //Quaternion<double> quat = rotZXY.ToQuat();
    //EulerAngles rot;
    //rot.FromQuat(quat);
    //Matrix4<double> rotMat = rotZYX.GetRotationMatrix();

    Transform tr = {{2,1,1}, Rotator::FromEuler(0,0,0), {1,1,1}};

    Matrix4<float> mat = tr.GetTransformMatrix();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << i << j << ":" << mat[j][i] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}