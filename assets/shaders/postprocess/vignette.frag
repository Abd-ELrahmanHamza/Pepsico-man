#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //TODO: Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?
    // Hint: the texture coordinate is the pixel's location in the texture divided by the texture's size

    /**
        What is the NDC space?
        it is a normalized device coordinate space, which is a 2D space that ranges from -1 to 1 used as a 
        final stage of a graphics pipeline. 

        after #D model is transformed and projected onto a 2D screen, the resulting x and y coordinates of each vertex
        are typically in device coordinate. 
        which are specified in pixels of the display hardware.
        it differs from screen to another based on the resolution of the screen.
    */
    // we need to divide the scene color by 1 + squared length of @D pixel location in NDC space
    frag_color = texture(tex, tex_coord);
    // float x = tex_coord.x * 2 - 1;
    // float y = tex_coord.y * 2 - 1;
    // float length = x * x + y * y;
    // frag_color = newcolor / (1 + length);

    frag_color = frag_color / (1 + length(2 * tex_coord - 1) * length(2 * tex_coord - 1));
    
}