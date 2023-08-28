#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>  // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

using namespace std;

#ifdef _WIN32
#include <windows.h>
#undef max
#undef min
#undef DrawText
#endif

#ifndef __APPLE__
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define __gl_h_ /* Prevent inclusion of the old gl.h */
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

// From: https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__, __LINE__)

#endif

/* VertexArrayObject */

class VertexArrayObject
{
public:
  GLuint id;

  VertexArrayObject() : id(0) {}

  /* New VAO */
  void init();
  void init(GLuint num);

  /* Select VAO */
  void bind();

  /* Release VAO */
  void free();
};

/* BufferObject */

class BufferObject
{
public:
  GLuint id;
  GLuint rows;
  GLuint cols;
  GLenum type;

  BufferObject() : id(0), rows(0), cols(0), type(GL_ARRAY_BUFFER) {}

  /* New BO */
  void init(GLenum t = GL_ARRAY_BUFFER);

  /* Update BO Data */
  template <typename T>
  void update(const vector<T> &array)
  {
    assert(id != 0);
    assert(!array.empty());
    glBindBuffer(type, id);
    glBufferData(type, sizeof(T) * array.size(), array.data(), GL_DYNAMIC_DRAW);
    cols = array.size();
    rows = array[0].length();
    check_gl_error();
  };

  /* Select BO */
  void bind();

  /* Release BO */
  void free();
};

/* Shader Program */

class Program
{
public:
  typedef unsigned int GLuint;
  typedef int GLint;

  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint programShader;

  Program() : vertexShader(0), fragmentShader(0), programShader(0) {}
  Program(GLuint id) : vertexShader(0), fragmentShader(0), programShader(id) {}

  /* New Shader Program With Vertex And Fragment */
  bool init(const string &vertexShaderString,
            const string &fragmentShaderString,
            const string &fragmentData);

  bool init(const string &vertexShaderString,
            const string &fragmentShaderString,
            const string &fragmentData1,
            const string &fragmentData2);

  bool init(const string &vertexShaderString,
            const string &fragmentShaderString,
            const string &fragmentData1,
            const string &fragmentData2,
            const string &fragmentData3);

  /* Select Shader Program */
  void bind();

  /* Release Shader Program */
  void free();

  /* Return the OpenGL handle of a named shader attribute (-1 if it does not exist) */
  GLint attrib(const string &name) const;

  /* Return the OpenGL handle of a uniform attribute (-1 if it does not exist) */
  GLint uniform(const string &name) const;

  /* Bind a per-vertex array attribute */
  GLint bindVertexAttribArray(const string &name, BufferObject &VBO) const;

  GLuint create_shader_helper(GLint type, const string &shaderString);
};
