#version 330 core
out vec4 FragColor;

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
//uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct FlashLight {
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform FlashLight flashLight;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 calcFlashLight(FlashLight light, vec3 normal, vec3 fragPos);

void main()
{
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    vec3 norm = normalize(Normal);

//    color += calcDirLight(dirLight, norm, FragPos);
    color += calcPointLight(pointLight, norm, FragPos);
    color += calcFlashLight(flashLight, norm, FragPos);

    FragColor = vec4(color, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 fragPos)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcFlashLight(FlashLight light, vec3 normal, vec3 fragPos)
{
    //    vec3 lightDir = normalize(LightPos - FragPos);
    //    vec3 lightDir = normalize(-light.direction);

    vec3 lightDir = normalize(-fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0f, 1.0f);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = lightDir;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    float distance = length(- fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}