#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
// da b3rf el vertex shader en ana hab3tlo normal attribute 34an ye3rf beh el direction
layout(location = 3) in vec3 normal;

// 34an nesbt el normal 3la el object w mylfsh m3 el lf bta3 el object, bnfsl el M mn el VP, w bnb3t kol wahed lwhdo
uniform mat4 VP;

// bnb3t el camera position 34an  a3rf mkan el object fen fl world blnesba lel camera
uniform vec3 camera_position;
uniform mat4 M;

// model inverse transpose, da 34an lama agy a3ml scale lel object, el normals hya kaman by7slhash scale
uniform mat4 M_IT;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

void main() {
    // b3rf el world 3n tre2 el M matrix
    vec3 world = (M * vec4(position, 1.0)).xyz;
    gl_Position = VP * vec4(world, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    // w b3den 34an ntl3 el object bdl mkan 4d n5leh 3d
    // bn5ly el vector ne3mlo v4, w b7ot el w b 0, w b3den
    // ba5ud el xyz bs.  
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);
    // keda gably el vector mn el object lel camera, fa da keda el view
    vs_out.view = camera_position - world;
    vs_out.world = world;
}