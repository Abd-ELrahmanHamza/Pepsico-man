#version 330 core

// @Author: Abdelaziz Salah Mohammed 
// @date: 24/3/2023
// @description: This is the vertex shader for the first assignment of the Computer Graphics course at the Faculty of Engineering, Cairo University
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

out Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 3) Change the next line to apply the transformation matrix
    // to the vertex position
    // lazm t7ot el result gowa gl_Position
    // w da 3n tre2 enna bndrb el transform fe position 34an nen2lha w n3mlha rotation w scaling w translation
    // w bma en position de aslun vec3, fa lazm ne3mlha casting. 
    gl_Position = transform * vec4(position, 1.0);


    // No need to change any of the following lines
    vs_out.position = position; 
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    vs_out.normal = normal;
}