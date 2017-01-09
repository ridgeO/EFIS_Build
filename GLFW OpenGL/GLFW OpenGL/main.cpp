//
//  main.cpp
//  GLFW OpenGL
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

int main()
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "OPEN GL WINDOW TITLE", nullptr, nullptr );
    
    int screenWIDTH, screenHEIGHT;
    glfwGetFramebufferSize( window, &screenWIDTH, &screenHEIGHT );
    
    if ( nullptr == window )
    {
        std::cout << "failed to create OpenGL window" <<std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    glewExperimental = GL_TRUE;
    
    if ( GLEW_OK != glewInit() )
    {
        std::cout << "failed to initialize GLEW" <<std::endl;
        return -1;
    }
    
    glViewport( 0, 0, screenWIDTH, screenHEIGHT );
    
    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents();
        glClearColor( 0, 0, 0, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        glfwSwapBuffers( window );
    }
    
    glfwTerminate();
    return 0;
}
