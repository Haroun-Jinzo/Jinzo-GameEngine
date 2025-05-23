#version 330 core

layout (location = 0) in vec3 aPos;       // Vertex position
layout (location = 1) in vec3 aOffset;    // 
layout (location = 2) in vec3 aSize;      //

uniform mat4 model;     // Model transformation matrix
uniform mat4 view;      // View transformation matrix
uniform mat4 projection; // Projection transformation matrix

void main()
{
    vec3 pos = aPos * aSize;

    // Transform the vertex position to clip space
    gl_Position = projection * view * model * vec4(pos + aOffset, 1.0f);
}