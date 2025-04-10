#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 0) out vec2 outUV;


void main() 
{
    vec2 pos = vec2(
        float(gl_VertexIndex & 1) * 2.0 - 1.0,
        float((gl_VertexIndex & 2) >> 1) * 2.0 - 1.0
    );

    outUV = pos * 0.5 + 0.5;
    gl_Position = vec4(pos, 0.0, 1.0);
}