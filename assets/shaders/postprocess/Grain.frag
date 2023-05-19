#version 330 core

/*
@Author: Abdelaziz Salah
@Date: 19/5/2023
@desc: This file is used to apply the Grain Noie scene. 

*/

uniform sampler2D tex; // Input texture

in vec2 tex_coord; // Texture coordinates

out vec4 frag_color; // Fragment shader output

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float grainAmount = 0.4; // Amount of grain effect


    // Sample the color from the input texture
    vec4 color = texture(tex, tex_coord);

    // Generate random noise
    float noise = rand(tex_coord);

    // Apply the grain effect
    vec3 grain = vec3(noise * grainAmount);

    // Add the grain to the color
    vec3 finalColor = color.rgb + grain;

    // Output the final color
    frag_color = vec4(finalColor, color.a);
}
