// @author Abdelaziz Salah

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

    /*
       how can we compute the pixel's location in the NDC space?
           First, convert the texture coordinates from pixel coordinates to normalized texture coordinates by dividing each coordinate by the texture dimensions. For example, if the texture is 512x512 pixels and the texture coordinate is (256, 256), the normalized texture coordinate would be (0.5, 0.5).

            Next, apply any necessary transformations to the texture coordinates based on the texture mapping technique being used. For example, if the texture is being mapped using perspective correction, you would apply the perspective correction formula to the normalized texture coordinates.

            Then, use the texture coordinates to look up the corresponding position in the 3D space of the object being textured. This can be done using the texture coordinates to interpolate the vertex positions of the object's geometry, or by using other techniques such as ray tracing.

            Finally, transform the resulting 3D position from object space to clip space, and then to NDC space using the projection matrix.

        Overall, the process of computing the location of a pixel's texture coordinate in NDC involves several steps that depend on the specific texture mapping technique being used, as well as the geometry and projection matrix of the scene being rendered.
    */
    

    /**
        What is the NDC space?
        it is a normalized device coordinate space, which is a 2D space that ranges from -1 to 1 used as a 
        final stage of a graphics pipeline. 

        after 3D model is transformed and projected onto a 2D screen, the resulting x and y coordinates of each vertex
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