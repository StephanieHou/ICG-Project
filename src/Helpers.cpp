#include "Helpers.h"

#include <iostream>
#include <fstream>

using namespace std;

/* VertexArrayObject */

void VertexArrayObject::init()
{
  glGenVertexArrays(1, &id);
  check_gl_error();
}

void VertexArrayObject::init(GLuint num)
{
  glGenVertexArrays(num, &id);
  check_gl_error();
}

void VertexArrayObject::bind()
{
  glBindVertexArray(id);
  check_gl_error();
}

void VertexArrayObject::free()
{
  glDeleteVertexArrays(1, &id);
  check_gl_error();
}

/* BufferObject */

void BufferObject::init(GLenum t)
{
  glGenBuffers(1, &id);
  type = t;
  check_gl_error();
}

void BufferObject::bind()
{
  glBindBuffer(type, id);
  check_gl_error();
}

void BufferObject::free()
{
  glDeleteBuffers(1, &id);
  check_gl_error();
}

/* Program */

bool Program::init(
    const string &vertexShaderString,
    const string &fragmentShaderString,
    const string &fragmentData)
{
  vertexShader = create_shader_helper(GL_VERTEX_SHADER, vertexShaderString);
  fragmentShader = create_shader_helper(GL_FRAGMENT_SHADER, fragmentShaderString);

  if (!vertexShader || !fragmentShader)
  {
    return false;
  }

  programShader = glCreateProgram();

  glAttachShader(programShader, vertexShader);
  glAttachShader(programShader, fragmentShader);

  glBindFragDataLocation(programShader, 0, fragmentData.c_str());
  glLinkProgram(programShader);

  GLint status;

  glGetProgramiv(programShader, GL_LINK_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];
    glGetProgramInfoLog(programShader, 512, NULL, buffer);
    cerr << "Linker error: " << endl
         << buffer << endl;
    programShader = 0;
    return false;
  }

  check_gl_error();
  return true;
}

bool Program::init(
    const string &vertexShaderString,
    const string &fragmentShaderString,
    const string &fragmentData1,
    const string &fragmentData2)
{
  vertexShader = create_shader_helper(GL_VERTEX_SHADER, vertexShaderString);
  fragmentShader = create_shader_helper(GL_FRAGMENT_SHADER, fragmentShaderString);

  if (!vertexShader || !fragmentShader)
  {
    return false;
  }

  programShader = glCreateProgram();

  glAttachShader(programShader, vertexShader);
  glAttachShader(programShader, fragmentShader);

  glBindFragDataLocation(programShader, 0, fragmentData1.c_str());
  glBindFragDataLocation(programShader, 1, fragmentData2.c_str());
  glLinkProgram(programShader);

  GLint status;

  glGetProgramiv(programShader, GL_LINK_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];
    glGetProgramInfoLog(programShader, 512, NULL, buffer);
    cerr << "Linker error: " << endl
         << buffer << endl;
    programShader = 0;
    return false;
  }

  check_gl_error();
  return true;
}

bool Program::init(
    const string &vertexShaderString,
    const string &fragmentShaderString,
    const string &fragmentData1,
    const string &fragmentData2,
    const string &fragmentData3)
{
  vertexShader = create_shader_helper(GL_VERTEX_SHADER, vertexShaderString);
  fragmentShader = create_shader_helper(GL_FRAGMENT_SHADER, fragmentShaderString);

  if (!vertexShader || !fragmentShader)
  {
    return false;
  }

  programShader = glCreateProgram();

  glAttachShader(programShader, vertexShader);
  glAttachShader(programShader, fragmentShader);

  glBindFragDataLocation(programShader, 0, fragmentData1.c_str());
  glBindFragDataLocation(programShader, 1, fragmentData2.c_str());
  glBindFragDataLocation(programShader, 2, fragmentData3.c_str());
  glLinkProgram(programShader);

  GLint status;

  glGetProgramiv(programShader, GL_LINK_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];
    glGetProgramInfoLog(programShader, 512, NULL, buffer);
    cerr << "Linker error: " << endl
         << buffer << endl;
    programShader = 0;
    return false;
  }

  check_gl_error();
  return true;
}

void Program::bind()
{
  glUseProgram(programShader);
  check_gl_error();
}

GLint Program::attrib(const string &name) const
{
  return glGetAttribLocation(programShader, name.c_str());
}

GLint Program::uniform(const string &name) const
{
  return glGetUniformLocation(programShader, name.c_str());
}

GLint Program::bindVertexAttribArray(const string &name, BufferObject &VBO) const
{
  GLint id = attrib(name);

  if (id < 0)
  {
    return id;
  }

  if (VBO.id == 0)
  {
    glDisableVertexAttribArray(id);
    return id;
  }

  VBO.bind();
  glEnableVertexAttribArray(id);
  glVertexAttribPointer(id, VBO.rows, GL_FLOAT, GL_FALSE, 0, 0);
  check_gl_error();

  return id;
}

void Program::free()
{
  if (programShader)
  {
    glDeleteProgram(programShader);
    programShader = 0;
  }

  if (vertexShader)
  {
    glDeleteShader(vertexShader);
    vertexShader = 0;
  }

  if (fragmentShader)
  {
    glDeleteShader(fragmentShader);
    fragmentShader = 0;
  }

  check_gl_error();
}

GLuint Program::create_shader_helper(GLint type, const string &shaderString)
{
  if (shaderString.empty())
  {
    return (GLuint)0;
  }

  GLuint id = glCreateShader(type);

  const char *constShaderString = shaderString.c_str();
  glShaderSource(id, 1, &constShaderString, NULL);
  glCompileShader(id);

  GLint status;

  glGetShaderiv(id, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];

    if (type == GL_VERTEX_SHADER)
    {
      cerr << "Vertex shader:" << endl;
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
      cerr << "Fragment shader:" << endl;
    }
    else if (type == GL_GEOMETRY_SHADER)
    {
      cerr << "Geometry shader:" << endl;
    }

    cerr << shaderString << endl
         << endl;

    glGetShaderInfoLog(id, 512, NULL, buffer);

    cerr << "Error: " << endl
         << buffer << endl;

    return (GLuint)0;
  }

  check_gl_error();

  return id;
}

/* Error */

void _check_gl_error(const char *file, int line)
{
  GLenum err(glGetError());

  while (err != GL_NO_ERROR)
  {
    string error;

    switch (err)
    {
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    }

    cerr << "GL_" << error.c_str() << " - " << file << ":" << line << endl;
    err = glGetError();
  }
}
