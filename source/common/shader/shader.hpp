#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram()
        {
            // (Req 1) Create A shader program
            program = glCreateProgram(); // create shader program using glCreateProgram
        }
        ~ShaderProgram()
        {
            // (Req 1) Delete a shader program
            glDeleteProgram(program); // delete shader program using glDeleteProgram
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // (Req 1) Send the given float value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform); // get uniform location of uniform using getUniformLocation
            glUniform1f(uniformLocation, value);                  // passing value GLfloat to uniform using glUniform1f
        }

        void set(const std::string &uniform, GLuint value)
        {
            // (Req 1) Send the given unsigned integer value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform); // get uniform location of uniform using getUniformLocation
            glUniform1ui(uniformLocation, value);                 // passing value GLuint to uniform using glUniform1ui
        }

        void set(const std::string &uniform, GLint value)
        {
            // (Req 1) Send the given integer value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform); // get uniform location of uniform using getUniformLocation
            glUniform1i(uniformLocation, value);                  // passing value GLint to uniform using glUniform1i
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // (Req 1) Send the given 2D vector value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform); // get uniform location of uniform using getUniformLocation
            glUniform2fv(uniformLocation, 1, &value[0]);          // passing value glm::vec2 to uniform using glUniform2fv
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // (Req 1) Send the given 3D vector value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform); // get uniform location of uniform using getUniformLocation
            glUniform3fv(uniformLocation, 1, &value[0]);          // passing value glm::vec3 to uniform using glUniform3fv
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // (Req 1) Send the given 4D vector value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform); // get uniform location of uniform using getUniformLocation
            glUniform4fv(uniformLocation, 1, &value[0]);          // passing value glm::vec4 to uniform using glUniform4fv
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // (Req 1) Send the given matrix 4x4 value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);         // get uniform location of uniform using getUniformLocation
            glUniformMatrix4fv(uniformLocation, 1, false, &matrix[0][0]); // passing value glm::mat4 to uniform using glUniformMatrix4fv
        }

        // (Req 1) Delete the copy constructor and assignment operator.
        // Question: Why do we delete the copy constructor and assignment operator?
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        // Answer:  you don't want to make a suitable copy ctor and cpy assignment operator to allocate a new memory
        //          to avoid having two pointers pointing to the same thing we delete the copy ctor and cpy assignment operator.,
        //          the unique ownership of resource should be encapsulated in a unique_ptr member
    };

}

#endif