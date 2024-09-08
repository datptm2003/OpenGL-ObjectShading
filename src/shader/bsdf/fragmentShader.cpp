const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Light {
    vec3 position;
    vec3 intensity;
};

uniform Light light;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform float albedo;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float cosTheta = max(dot(norm, lightDir), 0.0);

    float distance = length(light.position - FragPos);
    vec3 radiance = light.intensity / (distance * distance);
    vec3 diffuse = radiance * cosTheta * albedo * objectColor;

    FragColor = vec4(diffuse, 1.0);
}
)";