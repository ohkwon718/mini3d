#version 330 core

in vec3 vNormal;
in vec3 vWorldPosition;
in vec2 vTexCoord;
out vec4 FragColor;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform vec3 uBaseColor;
uniform vec3 uCameraPosition;
uniform float uShininess;
uniform float uSpecularStrength;
uniform sampler2D uTexture;
uniform bool uUseTexture;

void main()
{   
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightDirection);
    vec3 V = normalize(uCameraPosition - vWorldPosition);
    vec3 H = normalize(L+V);

    float diffuse = max(dot(N, L), 0.0);
    float specular = 0.0;
    if (diffuse > 0.0) {
        specular = pow(max(dot(N, H), 0.0), uShininess);
    }

    vec4 texColor = texture(uTexture, vTexCoord);

    vec3 albedo = uBaseColor;
    float alpha = 1;

    if (uUseTexture) {
        albedo *= texture(uTexture, vTexCoord).rgb;
        alpha = texColor.a;
    }

    float ambient = 0.15;
    vec3 ambientContribution = albedo * ambient;
    vec3 diffuseContribution = albedo * diffuse * uLightColor * uLightIntensity;
    vec3 specularContribution = specular * uSpecularStrength * uLightColor * uLightIntensity;
    FragColor = vec4(
        ambientContribution + diffuseContribution + specularContribution, 
        alpha
    );
}