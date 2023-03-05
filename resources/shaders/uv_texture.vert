#version 460 core

layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec3 i_Normal;
layout (location = 2) in vec2 i_UVCoords;
layout (location = 3) in vec3 i_Color;

layout (location = 0) out vec2 o_UVCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec2 u_UVCoords;

void main()
{
    if(u_UVCoords.x != 0.0 || u_UVCoords.y != 0.0)
    {
        o_UVCoords = u_UVCoords;
    }
    else
    {
        o_UVCoords = i_UVCoords;
    }
    gl_Position = u_Projection * u_View * u_Model * vec4(i_Pos, 1.0);
//    gl_Position = u_Model * vec4(i_Pos, 1.0);
}