#version 410 core

in vec3 vFragPos;
in vec3 vNormal;
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
uniform sampler2DShadow shadowMap; 

float getShadow(vec4 sc, float bias)
{
    vec3 proj = sc.xyz / sc.w;
    proj = proj * 0.5 + 0.5;
    // if (proj.z > 1.0) return 1.0;
    // if (proj.x < 0.0 || proj.x > 1.0 || proj.y < 0.0 || proj.y > 1.0) return 1.0;

    float shadow = 0.0;
    float texel = 1.0 / 512.0; 

    for(int x = -1; x <= 1; x++)
    for(int y = -1; y <= 1; y++)
        shadow += texture(shadowMap, vec3(proj.xy + vec2(x,y) * texel, proj.z - bias));

    return shadow / 9.0;
}


void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lpos.xyz - vFragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 V = normalize(cpos.xyz - vFragPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), 32.0);

    vec3 ambient = lamb.rgb * color.rgb;
    vec3 diffuse = ldif.rgb * color.rgb * diff;
    vec3 specular = lspe.rgb * spec;

    // compute bias and shadow factor (use slightly larger minimum bias to reduce acne)
    float bias = max(0.001, 0.005 * (1.0 - max(dot(N, L), 0.0)));
    float shadow = getShadow(vShadowCoord, bias);

    vec3 lighting = ambient + (diffuse + specular) * shadow;
    if (isEmissive == 1) lighting += emissionColor;

    vec4 texColor = texture(decal, vTexCoord);
    vec3 result = lighting * texColor.rgb;
    float alpha = color.a * texColor.a;

    outcolor = vec4(result, alpha);
}


// void main() {
//     vec3 proj = vShadowCoord.xyz / vShadowCoord.w;
//     proj = proj * 0.5 + 0.5;
//     float depth = texture(shadowMap, vec3(proj.xy, proj.z));

//     outcolor = vec4(vec3(depth), 1.0);
// }

