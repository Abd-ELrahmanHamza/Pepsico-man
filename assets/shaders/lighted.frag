#version 330

#define DIRECTIONAL 0
#define POINT       1
#define SPOT        2

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color; // amibent w specular w el diffusion 
    vec3 attenuation; // kol ma nb3d el do2 el mfrod yed3f
    vec2 cone_angles; // spot light
};

#define MAX_LIGHTS 80

uniform Light lights[MAX_LIGHTS];
uniform int light_count;

// ambient light, enhwa bymsl kowet el do2 mn kol etgah, swa2 top aw horizon aw bottom.
struct Sky {
    vec3 top, horizon, bottom;
};

uniform Sky sky;

vec3 compute_sky_light(vec3 normal){
    vec3 extreme = normal.y > 0 ? sky.top : sky.bottom;
    return mix(sky.horizon, extreme, normal.y * normal.y);
}

// bt3br 3n el object bta3na.  -> texture mix.
struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emissive;
};

uniform Material material;

// de el 7agat elly gayaly mn el vertex shader.
in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

// de el 7aga elly bn5rgha mn el fragment shader bt3na
out vec4 frag_color;

// de bt3br 3n el phong model. 
float lambert(vec3 normal, vec3 world_to_light_direction) {
    return max(0.0, dot(normal, world_to_light_direction));
}


float phong(vec3 reflected, vec3 view, float shininess) {
    return pow(max(0.0, dot(reflected, view)), shininess);
}

void main() {
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);
    
    vec3 ambient_light = compute_sky_light(normal);

    vec3 diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 specular = texture(material.specular, fs_in.tex_coord).rgb;
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    vec3 ambient = diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    vec3 emissive = texture(material.emissive, fs_in.tex_coord).rgb;

    float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    
    vec3 world_to_light_dir;
    vec3 color = emissive + ambient_light * ambient;

    for(int light_idx = 0; light_idx < min(MAX_LIGHTS, light_count); light_idx++){
        Light light = lights[light_idx];
        float attenuation = 1.0;
        if(light.type == DIRECTIONAL){
            world_to_light_dir = -light.direction;
        } else {
            world_to_light_dir = light.position - fs_in.world;
            float d = length(world_to_light_dir);
            world_to_light_dir /= d;
            attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));
            if(light.type == SPOT){
                float angle = acos(dot(light.direction, -world_to_light_dir));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        vec3 computed_diffuse = light.color * diffuse * lambert(normal, world_to_light_dir);

        vec3 reflected = reflect(-world_to_light_dir, normal);
        vec3 computed_specular = light.color * specular * phong(reflected, view, shininess);

        color += (computed_diffuse + computed_specular) * attenuation;

    }
    
    frag_color = vec4(color, 1.0);
}