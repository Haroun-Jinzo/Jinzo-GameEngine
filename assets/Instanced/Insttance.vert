#version 330 core

layout (location = 0) in vec3 aPos;       // Vertex position
layout (location = 1) in vec3 aNormal;    // Vertex normal
layout (location = 2) in vec2 aTexCoord;  // Texture coordinates
layout (location = 3) in vec3 aOffset;    // 
layout (location = 4) in vec3 aSize;      //

out vec3 FragPos;      // Passes the fragment position to the fragment shader
out vec3 Normal;       // Passes the normal to the fragment shader
out vec2 texCoord;     // Passes the texture coordinates to the fragment shader

uniform mat4 model;     // Model transformation matrix
uniform mat4 view;      // View transformation matrix
uniform mat4 projection; // Projection transformation matrix

void main()
{
    vec3 pos = aPos * aSize;

    // Transform the vertex position to world space
    FragPos = vec3(model * vec4(aPos + aOffset, 1.0f));

    // Transform the normal to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass the texture coordinates
    texCoord = aTexCoord;

    // Transform the vertex position to clip space
    gl_Position = projection * view * vec4(FragPos, 1.0f);
}