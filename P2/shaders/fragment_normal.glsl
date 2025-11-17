#version 410 core

in vec3 vFragPos;
in vec3 vNormal;
in vec3 vTangent;
in vec2 vTexCoord;
in vec4 vShadowCoord;

out vec4 outcolor;

uniform vec4 color;
uniform vec4 lpos;
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;
uniform vec4 cpos;
uniform vec3 emissionColor;

uniform int isEmissive;

uniform sampler2D decal;
uniform sampler2D normalMap;
uniform sampler2DShadow shadowMap; 

float getShadow(vec4 sc)
{
    vec3 proj = sc.xyz / sc.w;
    proj = proj * 0.5 + 0.5;

    if (proj.z > 1.0) return 1.0;

    float shadow = 0.0;
    float texel = 1.0 / 2048.0; 

    for(int x = -1; x <= 1; x++)
    for(int y = -1; y <= 1; y++)
        shadow += texture(shadowMap, vec3(proj.xy + vec2(x,y) * texel, proj.z));

    return shadow / 9.0;
}


void main() {
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    vec3 mapN = texture(normalMap, vTexCoord).rgb;
    mapN = mapN * 2.0 - 1.0;
    N = normalize(TBN * mapN);

    vec3 L = normalize(lpos.xyz - vFragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 V = normalize(cpos.xyz - vFragPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), 32.0);

    float distance = length(lpos.xyz - vFragPos);

    vec3 ambient = lamb.rgb * color.rgb;
    vec3 diffuse = ldif.rgb * color.rgb * diff;
    vec3 specular = lspe.rgb * spec;


    // float shadow = getShadow(vShadowCoord);
    vec3 lighting = ambient + (diffuse + specular);

    if (isEmissive == 1) lighting += emissionColor;
    vec3 result;
    float alpha;

    vec4 texColor = texture(decal, vTexCoord);
    result = lighting * texColor.rgb;
    alpha = color.a * texColor.a;


    outcolor = vec4(result, alpha);
}
