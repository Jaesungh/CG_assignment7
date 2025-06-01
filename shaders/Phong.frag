#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

// light properties
uniform vec3 lightAmbient;   // Ia
uniform vec3 lightDiffuse;   // Il
uniform vec3 lightSpecular;  // Il

// material properties
uniform vec3 matAmbient;     // ka
uniform vec3 matDiffuse;     // kd
uniform vec3 matSpecular;    // ks
uniform float shininess;     // p

uniform float gamma;        

out vec4 FragColor;

void main()
{
    //  ambient term: Ia * ka
    vec3 ambient = lightAmbient * matAmbient;

    //  diffuse term: Il * kd * max(dot(N, L), 0)
    vec3 L = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, L), 0.0);
    vec3 diffuse = lightDiffuse * (diff * matDiffuse);

    // specular term (Blinn-Phong): Il * ks * (max(dot(N, H),0)^p)
    vec3 V = normalize(viewPos - FragPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(Normal, H), 0.0), shininess);
    vec3 specular = lightSpecular * (spec * matSpecular);

    //  linear 색상 계산
    vec3 color_lin = ambient + diffuse + specular;

    //  감마 보정 (RGB 각각에 1/gamma 증가)
    vec3 color = pow(color_lin, vec3(1.0 / gamma));

    FragColor = vec4(color, 1.0);
}