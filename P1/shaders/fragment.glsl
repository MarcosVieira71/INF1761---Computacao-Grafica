#version 410 core
in vec3 vFragPos;
in vec3 vNormal;
in vec2 tex;

out vec4 outcolor;

uniform vec4 color;       // cor base do objeto
uniform vec4 lpos;        // posição da luz
uniform vec4 lamb;        // luz ambiente
uniform vec4 ldif;        // luz difusa
uniform vec4 lspe;        // luz especular
uniform sampler2D decal;  // textura

void main() {
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lpos.xyz - vFragPos);

    vec3 ambient = lamb.rgb * color.rgb;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = ldif.rgb * diff * color.rgb;

    vec3 viewDir = normalize(-vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = lspe.rgb * spec;

    vec3 lighting = ambient + diffuse + specular;

    vec4 texColor = texture(decal, tex);
    vec3 result = lighting * texColor.rgb;

    outcolor = vec4(result, color.a * texColor.a);
}
