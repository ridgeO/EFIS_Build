//
//  main.cpp
//  EFIS_V2
//
//  Created by Ridge on 1/9/17.
//  Copyright © 2017 Ridge. All rights reserved.
//

#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;
const GLchar *vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4( position.x, position.y, position.z, 1.0 );\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4( 1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

int main()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "EFIS_V2", nullptr, nullptr );
    
    int screenWIDTH, screenHEIGHT;
    glfwGetFramebufferSize( window, &screenWIDTH, &screenHEIGHT );
    
    if ( nullptr == window )
    {
        std::cout << "failed to create OpenGL window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    // Set Parameters and Initialize GLEW
    glewExperimental = GL_TRUE;
    
    if ( GLEW_OK != glewInit() )
    {
        std::cout << "failed to initialize GLEW" <<std::endl;
        return -1;
    }
    
    // Define Viewport Dimensions
    glViewport( 0, 0, screenWIDTH, screenHEIGHT );
    
    // Shader Compilation
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
    GLint success;
    GLchar infoLog[512];
    
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success);
    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED/n" << infoLog << std::endl;
    }
    
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );
    
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success);
    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED/n" << infoLog << std::endl;
    }
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );
    
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
    if ( !success )
    {
        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED/n" << infoLog << std::endl;
    }
    
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
    
    // Create Shapes
    
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat), (GLvoid *) 0 );
    glEnableVertexAttribArray( 0 );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    
    // Run GLFW Window Loop
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents();
        glClearColor( 0, 0, 0, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Drawing Code goes here
        glUseProgram( shaderProgram );
        glBindVertexArray( VAO );
        glDrawArrays (GL_TRIANGLES, 0, 3 );
        glBindVertexArray( 0 );
        
        glfwSwapBuffers( window );
    }
    
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    
    glfwTerminate();
    return 0;
}
