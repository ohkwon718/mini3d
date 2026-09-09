#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 vWorldPosition;
out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{        
    vWorldPosition = (uModel * vec4(aPosition, 1.0)).xyz;
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    vNormal = normalize( mat3(transpose(inverse(uModel))) * aNormal );
    vTexCoord = aTexCoord;
}