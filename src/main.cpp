#include "../include/glad/glad.h"
#include "../include/Shader.h"
#include "../include/stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Callback function to adjust the viewport if the GLFW window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Process input in the window 
void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

int main()
{
  // Initialize GLFW 
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  
  // Initialize GLAD (OpenGL Function Pointers) before using any OpenGL Function
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Tell OpenGL the size of the rendering window so OpenGL knows 
  // how we want to display the data and coordinates with respect to the window
  glViewport(0, 0, 800, 600);

  // Callback function if GLFW window is resized
  glfwSetWindowSizeCallback(window, framebuffer_size_callback);
  
  // Get the number of vertex attributes available on this hardware 
  int numberOfVertexAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numberOfVertexAttributes);
  std::cout << "Maximum number of vertex attributes supported: " << numberOfVertexAttributes << std::endl;
  
  
  //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
  // Begin Shaders, Vertex Buffer Objects, Vertex Array Objects 

  // 3 vertices x, y, z 
  float vertices[] = {
    // Positions          // Colors            // Texture coordinates
     0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,   // Top right 
     0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  // Bottom right 
    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // bottom left 
    -0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f   // top left 
  };

  unsigned int indices[] = { // note that we start from 0! 
    0, 1, 3, // first triangle
    1, 2, 3
  };

  // VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generate VBO 
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // EBO -- Element Array Buffer Object
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Shader class 
  Shader ourShader("../shaders/shader.vs", "../shaders/shader.fs");

  // Linking Vertex Attributes
  // Position attributes 
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // Color attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture Attributes
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);
/*
  // Vertex shader 
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check if the vertex shader compiled successfully
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Fragment Shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check if the fragment shader compiled successfully
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Shader Program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check if the shader program failed 
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if(!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Clean up shaders are compiling and linking them 
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

*/

  // End Shaders, Vertex Buffer Objects, Vertex Array Objects
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  // TEXTURES &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

  unsigned int texture1; 
  unsigned int texture2; 
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // Set the texture wrapping / filtering options (on the currently bound texture) 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load and generate the texture using the stb_image library 
  int width, height, nrChannels;
  std::string filePath = "../assets/container.jpg";
  unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else 
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  // free the image memory
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // Set the texture wrapping / filtering options (on the currently bound texture) 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  filePath = "../assets/awesomeface.png";
  stbi_set_flip_vertically_on_load(false);
  data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else 
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  // free the image memory
  stbi_image_free(data);

  // Tell OpenGL which texture unit each shader sampler belongs to by setting each sampler using glUniform1i
  // Only have to set this once so we can do it before entering the render loop 
  ourShader.use();
  glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);  // do it manually with gl
  ourShader.setInt("texture2", 1); // or with our shader class function 

  // GLFW Render Loop!
  while(!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // Rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Custom color for fragmentShader

    // Use the program 
    // glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up GLFW resources
  glfwTerminate();
  return 0;

}
