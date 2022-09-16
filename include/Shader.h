#ifndef SHADER_H
#define SHADER_H

#include "./glad/glad.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  // The program ID
  unsigned int ID;

  // Constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath);
  // use / activate the shader
  void use();
  // Utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
