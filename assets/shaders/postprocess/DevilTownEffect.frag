#version 330 core
/*
@Author: Abdelaziz Salah
@Date: 19/5/2023
@desc: This file is used to apply the devil town scene. 

*/

uniform sampler2D tex; // Input texture
// uniform vec3 fogColor; // Color of the fog
vec3 devilColor; // Color of the fog
// uniform float devilDensity; // Density of the fog
float devilDensity; // Density of the fog

in vec2 tex_coord; // Texture coordinates

out vec4 frag_color; // Fragment shader output

void main()
{
    devilColor = vec3(0.5, 0.5, 0.35);
    devilDensity = 100; ; 
    // Sample the color from the input texture
    vec4 color = texture(tex, tex_coord);

    // Calculate the fog factor based on the distance from the camera
    float fogDistance = (-gl_FragCoord.z ) /( gl_FragCoord.w); // Linear depth
    float fogFactor = exp(-devilDensity * fogDistance );

    // Blend the color with the fog color based on the fog factor
    vec3 blendedColor = mix(color.rgb, devilColor, fogFactor);

    // Output the final color
    frag_color = vec4(blendedColor, color.a);
}
