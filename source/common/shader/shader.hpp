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
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // (Req 1) Delete a shader program
            glDeleteProgram(program);
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
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniform1f(uniformLocation, value);
        }

        void set(const std::string &uniform, GLuint value)
        {
            // (Req 1) Send the given unsigned integer value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniform1ui(uniformLocation, value);
        }

        void set(const std::string &uniform, GLint value)
        {
            // (Req 1) Send the given integer value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniform1i(uniformLocation, value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // (Req 1) Send the given 2D vector value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniform2fv(uniformLocation, 1, &value[0]);
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // (Req 1) Send the given 3D vector value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniform3fv(uniformLocation, 1, &value[0]);
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // (Req 1) Send the given 4D vector value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniform4fv(uniformLocation, 1, &value[0]);
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // (Req 1) Send the given matrix 4x4 value to the given uniform
            GLuint uniformLocation = getUniformLocation(uniform);
            glUniformMatrix4fv(uniformLocation, 1, false, &matrix[0][0]);
        }

        // (Req 1) Delete the copy constructor and assignment operator.
        // Question: Why do we delete the copy constructor and assignment operator?
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
    };

}

#endif