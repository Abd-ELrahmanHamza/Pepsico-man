#version 330 core

in Varyings {
    vec4 color;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color
    // frag_color = vec4(1.0);
    // frag_color = vec4(tint[0]*fs_in.color[0],tint[1]*fs_in.color[1],tint[2]*fs_in.color[2],tint[3]*fs_in.color[3]);
    frag_color = tint*fs_in.color;
}