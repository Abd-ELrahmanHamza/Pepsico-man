// @author: Abdelziz Salah
// @date: 17/3/2023
// requirement2: implement the mesh class and finish the TODOS.
#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        ////////////////////////////////////////////////////////////////////////////////
        ///// The vertex buffer and the element buffer
        unsigned int VBO, EBO;

        ///// The vertex array object
        unsigned int VAO;
        ////////////////////////////////////////////////////////////////////////////////
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            /// keda lazm  el buffers tartebhom yefr2
            /// kman lazm lama tegy t3ml el attribute pointer lazm be3ml kol el elements el gowaha ,
            /// fa hena hn3ml el color  wl coor wl norm w kol haga gowa, w 3ddhom byb2a 3la 7asab el vec3 aw vec2 aw el size 3moman.
            // TODO: (Req 2) Write this function
            // finally, defining the vertex array object
            glGenVertexArrays(1, &VAO);
            // binding the name
            glBindVertexArray(VAO);
            // creating a buffer
            glGenBuffers(1, &VBO);
            // binding the buffer
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // defining the data to be sent, and defining how to send them
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // enabling the attribute
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION); // enable the attribute

            // defining the data to be sent, and defining how to send them
            //                                                       true ?
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
            // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

            // color
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

            // tex coord
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

            // norm
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

            // defining the element buffer
            glGenBuffers(1, &EBO);
            // binding the name
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // defining the data to be sent, and defining how to send them
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            elementCount = elements.size();
        }

        // this function should render the mesh
        /*
            utility function to draw the mesh
        */
        void draw()
        {
            // TODO: (Req 2) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);
            // glswap buffer should be here?
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: (Req 2) Write this function
            /// UNBind
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}