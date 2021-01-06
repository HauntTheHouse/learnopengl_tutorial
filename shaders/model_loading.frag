#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;
uniform mat4 view;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);

void main()
{
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    vec3 norm = normalize(Normal);

    //    color += calcDirLight(dirLight, norm, FragPos);
    color += calcPointLight(pointLight, norm, FragPos);
//    color += calcFlashLight(flashLight, norm, FragPos);

    FragColor = vec4(color, 1.0f);}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}