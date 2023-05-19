#version 330 core
/*
@Author: Abdelaziz Salah
@Date: 19/5/2023
@desc: This file is used to apply the ramp to edge scene. 
*/

uniform sampler2D tex; // Input texture

in vec2 tex_coord; // Texture coordinates

out vec4 frag_color; // Fragment shader output

void main()
{
    float distortionStrength = 0.5; // Strength of the distortion effect

    // Define the distortion center based on implementation
    vec2 distortionCenter = vec2(0.5, 0.5); // Modify these values based on your implementation

    // Calculate the vector from the distortion center to the current texture coordinate
    vec2 distortionVector = tex_coord - distortionCenter;

    // Calculate the dist from the distortion center
    float dist = length(distortionVector);

    // Apply the lens distortion effect
    vec2 distortedtex_coord = tex_coord + (distortionVector * (1.0 + distortionStrength * dist * dist));

    // Sample the color from the distorted texture coordinates
    vec4 color = texture(tex, distortedtex_coord);

    // Output the final color
    frag_color = color;
}
