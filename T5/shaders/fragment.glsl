#version 410 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 outcolor;

uniform vec4 color;
uniform vec4 lpos;
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;
uniform vec4 cpos;
uniform sampler2D decal;  
uniform int hasTexture;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lpos.xyz - vFragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 V = normalize(cpos.xyz - vFragPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), 16.0);

    vec3 ambient = lamb.rgb * color.rgb;
    vec3 diffuse = ldif.rgb * color.rgb * diff;
    vec3 specular = lspe.rgb * spec;
    
    vec3 lighting = ambient + diffuse + specular;
    
    vec4 texColor = texture(decal, vTexCoord);

    vec3 result;
    float alpha;

    if (hasTexture == 1) {
        result = lighting * texColor.rgb;
        alpha = color.a * texColor.a;
    } else {
        result = lighting;
        alpha = color.a;
    }

    outcolor = vec4(result, color.a * texColor.a);
}
