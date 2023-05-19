#version 330 core

/*
    @Author: Abdelaziz Salah
    @Date: 19/5/2023
    @desc: This file is used to apply the repeat the scene effect. 
*/

uniform sampler2D tex; // Input texture

in vec2 tex_coord; // Texture coordinates

out vec4 frag_color; // Fragment shader output

void main()
{
    float blurStrength = 0.5; // Strength of the motion blur effect
    vec2 blurDirection = vec2(0.1, 0.5); // Direction of the motion blur effect

    // Calculate the amount of blur displacement
    vec2 blurOffset = blurStrength * blurDirection;

    // Initialize the color accumulator
    vec4 accumulatedColor = vec4(0.0);

    // Number of samples for motion blur
    int numSamples = 10;

    // Perform motion blur by accumulating multiple texture samples
    for (int i = 0; i < numSamples; ++i)
    {
        // Calculate the texture coordinate for the current sample
        vec2 sampleCoord = tex_coord - float(i) * blurOffset;

        // Sample the color from the input texture
        vec4 color = texture(tex, sampleCoord);

        // Accumulate the color
        accumulatedColor += color;
    }

    // Average the accumulated color
    vec4 finalColor = accumulatedColor / float(numSamples);

    // Output the final color
    frag_color = finalColor;
}
