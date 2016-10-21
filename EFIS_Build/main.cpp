//
//  main.cpp
//  EFIS_Build
//
//  Created by rto on 10/20/16.
//  Copyright © 2016 rto. All rights reserved.
//

// Load standard library
#include <stdlib.h>
// Load iostream library for standard functionality
//#include <iostream>
// Load math library for circle and roations
#include <math.h>
// Load GLEW for handling opnenGL functions
//#define GLEW_STATIC
//#include <GL/glew.h>
// Load GLFW for handing openGL functions
//#include <GLFW/glfw3.h>
#include "glfw_app.hpp"
// Load FreeType for text processing
//#include <freetype2/ft2build.h>

#define PI 3.141593692

// Set default screen width and height which will be adjusted later based on hardware
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
    int i;
    int triangleAmount = 80; //# of triangles used to draw circle
    
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for(i = 0; i <= triangleAmount;i++) {
        glVertex2f(
                   x + (radius * cos(i *  twicePi / triangleAmount)),
                   y + (radius * sin(i * twicePi / triangleAmount))
                   );
    }
    glEnd();
}

void DrawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    float tangetial_factor = tanf(theta);
    float radial_factor = cosf(theta);
    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);
    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);
        float tx = -y;
        float ty = x;
        x += tx * tangetial_factor;
        y += ty * tangetial_factor;
        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}
/*
 using namespace lynda;
 
 struct MyApp : App {
 
 MyApp() : App() {}
 
 virtual void onDraw(){
 }
 
 virtual void onMouseMove(int x, int y){
 cout << x << " " << y << endl;
 }
 virtual void onMouseDown(int button, int action){
 if (action==GLFW_PRESS) cout << "mouse pressed" <<endl;
 if (action==GLFW_RELEASE) cout << "mouse released" <<endl;
 }
 virtual void onKeyDown(int key, int action){
 cout << (unsigned char)key << endl;
 }
 };
 
 
 int main(){
 
 MyApp app;
 app.start();
 
 printf("goodbye window\n");
 
 return 0;
 }
 */

// Begin main function
int main (void)
{
    // Initialize GLFW
    GLFWwindow *window;
    // Make sure library initializes and close out GLFW if not
    if (!glfwInit())
    {
        return -1;
    }
    // Set initial openGL window with dimensions and title
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "G1000 Build V4", NULL, NULL);
    glfwSetKeyCallback(window, keyCallback);
    // Make sure window initializes and close out OpenGL if not
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        printf("glew init error\n%s\n", glewGetErrorString(glewError));
    }
    if (!GLEW_VERSION_2_1)
    {
        printf("OpenGL V2.1 not supported!\n");
        return false;
    }
    
    
    // Get display dimensions
    // int screen_width, screen_height;
    // glfwGetFramebufferSize(window, &screen_width, &screen_height);
    
    // Set commonly used heights and widths
    float top_bound = SCREEN_HEIGHT;
    float bottom_bound = 0;
    float left_bound = 0;
    float right_bound = SCREEN_WIDTH;
    float center_x = SCREEN_WIDTH/2;
    float center_y = SCREEN_HEIGHT/2;
    float screen_width_f = SCREEN_WIDTH;
    float screen_height_f = SCREEN_HEIGHT;
    float horizon_origin = SCREEN_HEIGHT * .625;
    float horizon_block_width = screen_width_f * 2;
    float horizon_block_height = screen_height_f * 4;
    float horizon_block_leftbound = left_bound - .25 * horizon_block_width;
    float horizon_block_rightbound = right_bound + .5 * horizon_block_width;
    float horizon_line_thickness = SCREEN_HEIGHT * .0025;
    float bounding_box_height = screen_height_f * .05;
    float main_box_width = screen_width_f * .08;
    float main_box_height = screen_height_f * .45;
    float secondary_box_height = main_box_height * .8;
    float secondary_box_width = main_box_width * .4;
    float highlight_box_height = screen_height_f * .04;
    float ars_box_leftbound = center_x - float(3.75 * main_box_width);
    float ars_box_rightbound = ars_box_leftbound + main_box_width;
    float ars_box_topbound = float(horizon_origin + .5 * main_box_height);
    float ars_box_bottombound = float(horizon_origin - .5 * main_box_height);
    float alt_box_leftbound = center_x + float(2.75 * main_box_width);
    float alt_box_rightbound = alt_box_leftbound + main_box_width;
    float alt_box_topbound = float(horizon_origin + .5 * main_box_height);
    float alt_box_bottombound = float(horizon_origin - .5 * main_box_height);
    float chevron_width = screen_width_f * .2;
    float chevron_height = screen_height_f * .04;
    float chevron_leftbound = center_x - .5 * chevron_width;
    float chevron_rightbound = center_x + .5 * chevron_width;
    float chevron_topbound = horizon_origin;
    float chevron_bottombound = horizon_origin - chevron_height;
    float chevron_bottom_width = chevron_width * .25;
    float chevron_mid_thickness = chevron_height * .25;
    float dg_radius = screen_height_f * .21;
    float dg_center_x = center_x;
    float dg_center_y = bottom_bound + bounding_box_height + dg_radius;
    float current_heading_box_bottombound = dg_center_y + dg_radius;
    float current_heading_box_topbound = current_heading_box_bottombound + highlight_box_height;
    float current_heading_box_leftbound = center_x - dg_radius / 4;
    float current_heading_box_rightbound = center_x + dg_radius / 4;
    float current_heading_triangle_leftbound = center_x - dg_radius / 8;
    float current_heading_triangle_rightbound = center_x + dg_radius / 8;
    float current_heading_triangle_bottombound = current_heading_box_bottombound - sqrtf((dg_radius/4)*(dg_radius/4)-(dg_radius/8)*(dg_radius/8));
    
    //-----------------------------------
    // DEFINE COLORS
    //-----------------------------------
    #define WHITE 255,255,255
    #define BLACK 0,0,0
    #define HORIZON_BLOCK_TOP_COLOR 51,153,255
    #define HORIZON_BLOCK_BOTTOM_COLOR 139,69,19
    #define HORIZON_LINE_COLOR 224,224,224
    #define TOP_BOX_COLOR 32,32,32
    #define BOTTOM_BOX_COLOR 32,32,32
    #define CHEVRON_COLOR 255,215,0
    #define BOX_COLOR 205,205,205
    #define HIGHLIGHT_BOX_COLOR 32,32,32
    #define DG_COLOR 65,65,65
        
    //-----------------------------------
    // HORIZON BLOCKS
    //-----------------------------------
    
    // Set points for Horizon_block_bottom
    GLfloat Horizon_block_bottom[] =
    {
        horizon_block_leftbound,horizon_origin,0,
        horizon_block_rightbound,horizon_origin,0,
        horizon_block_rightbound,horizon_origin - horizon_block_height,0,
        horizon_block_leftbound,horizon_origin - horizon_block_height,0
    };
    
    // Set points for Horizon_block_top
    GLfloat Horizon_block_top[] =
    {
        horizon_block_leftbound,horizon_origin + horizon_block_height,0,
        horizon_block_rightbound,horizon_origin + horizon_block_height,0,
        horizon_block_rightbound,horizon_origin,0,
        horizon_block_leftbound,horizon_origin,0
    };
    
    // Set points for Horizon_block_line
    GLfloat Horizon_block_line[] =
    {
        horizon_block_leftbound,horizon_origin + horizon_line_thickness / 2,0,
        horizon_block_rightbound,horizon_origin + horizon_line_thickness / 2,0,
        horizon_block_rightbound,horizon_origin - horizon_line_thickness / 2,0,
        horizon_block_leftbound,horizon_origin - horizon_line_thickness / 2,0
    };
    
    //-------------------------------------
    // BOUNDING BOXES FOR FREQS & KEYS
    //-------------------------------------
    
    // Set points for Top_box_1
    GLfloat Top_box_1[] =
    {
        0,screen_height_f,0,
        screen_width_f,screen_height_f,0,
        screen_width_f,screen_height_f - bounding_box_height,0,
        0,screen_height_f - bounding_box_height,0
    };
    
    // Set points for Top_box_2
    GLfloat Top_box_2[] =
    {
        0,screen_height_f,0,
        screen_width_f,screen_height_f,0,
        screen_width_f,screen_height_f - 2 * bounding_box_height,0,
        0,screen_height_f - 2 * bounding_box_height,0
    };
    
    // Set point for Bottom_box
    GLfloat Bottom_box[] =
    {
        0,bounding_box_height,0,
        screen_width_f,bounding_box_height,0,
        screen_width_f,0,0,
        0,0,0
    };
    
    //------------------------------
    // ALTITUDE BOXES
    //------------------------------
    
    
    // Set points for Altitude_box
    GLfloat Altitude_box[] =
    {
        alt_box_leftbound,alt_box_topbound,0,
        alt_box_rightbound,alt_box_topbound,0,
        alt_box_rightbound,alt_box_bottombound,0,
        alt_box_leftbound,alt_box_bottombound,0
    };
    
    // Set points for Altitude_select_box
    GLfloat Altitude_select_box[] =
    {
        alt_box_leftbound,alt_box_topbound + highlight_box_height,0,
        alt_box_rightbound,alt_box_topbound + highlight_box_height,0,
        alt_box_rightbound,alt_box_topbound,0,
        alt_box_leftbound,alt_box_topbound,0
    };
    
    // Set points for Altitude_current_box
    GLfloat Altitude_current_box[] =
    {
        alt_box_leftbound,float(horizon_origin + .5 * highlight_box_height),0,
        alt_box_rightbound,float(horizon_origin + .5 * highlight_box_height),0,
        alt_box_rightbound,float(horizon_origin - .5 * highlight_box_height),0,
        alt_box_leftbound,float(horizon_origin - .5 * highlight_box_height),0
    };
    
    // Set points for Altitude_set_box
    GLfloat Altitude_set_box[] =
    {
        alt_box_leftbound,alt_box_bottombound,0,
        alt_box_rightbound,alt_box_bottombound,0,
        alt_box_rightbound,alt_box_bottombound - highlight_box_height,0,
        alt_box_leftbound,alt_box_bottombound - highlight_box_height,0
    };
    
    // Set points for Vertical_speed_box
    GLfloat Vertical_speed_box[] =
    {
        alt_box_rightbound,float(horizon_origin + .5 * secondary_box_height),0,
        alt_box_rightbound + secondary_box_width,float(horizon_origin + .5 * secondary_box_height),0,
        alt_box_rightbound + secondary_box_width,float(horizon_origin - .5 * secondary_box_height),0,
        alt_box_rightbound,float(horizon_origin - .5 * secondary_box_height),0
    };
    
    //------------------------------
    // AIRSPEED BOXES
    //------------------------------

    // Set points for Airspeed_box
    GLfloat Airspeed_box[] =
    {
        ars_box_leftbound,ars_box_topbound,0,
        ars_box_rightbound,ars_box_topbound,0,
        ars_box_rightbound,ars_box_bottombound,0,
        ars_box_leftbound,ars_box_bottombound,0
    };
    
    // Set points for Oat_box
    GLfloat Oat_box[] =
    {
        ars_box_leftbound,ars_box_topbound + highlight_box_height,0,
        ars_box_rightbound,ars_box_topbound + highlight_box_height,0,
        ars_box_rightbound,ars_box_topbound,0,
        ars_box_leftbound,ars_box_topbound,0
    };
    
    // Set points for Airspeed_current_box
    GLfloat Airspeed_current_box[] =
    {
        ars_box_leftbound,float(horizon_origin + .5 * highlight_box_height),0,
        ars_box_rightbound,float(horizon_origin + .5 * highlight_box_height),0,
        ars_box_rightbound,float(horizon_origin - .5 * highlight_box_height),0,
        ars_box_leftbound,float(horizon_origin - .5 * highlight_box_height),0
    };
    
    // Set points for Airspeed_tas_box
    GLfloat Airspeed_tas_box[] =
    {
        ars_box_leftbound,ars_box_bottombound,0,
        ars_box_rightbound,ars_box_bottombound,0,
        ars_box_rightbound,ars_box_bottombound - highlight_box_height,0,
        ars_box_leftbound,ars_box_bottombound - highlight_box_height,0
    };

    //------------------------------
    // CHEVRON
    //------------------------------
    
    GLfloat Chevron[] =
    {
        chevron_leftbound,chevron_bottombound,0,
        chevron_leftbound + chevron_bottom_width,chevron_bottombound,0,
        center_x,chevron_topbound,0,
        center_x,chevron_topbound - chevron_mid_thickness,0,
        chevron_rightbound - chevron_bottom_width,chevron_bottombound,0,
        chevron_rightbound,chevron_bottombound,0,
        center_x,chevron_topbound,0
    };
    
    // Set points for Chevron_left_box
    GLfloat Chevron_left_box[] =
    {
        
    };
    
    // Set points for Chevron_right_box
    GLfloat Chevron_right_box[] =
    {
        
    };
    
    // Set viewport and such
    glViewport(0.0f, 0.0f, screen_width_f, screen_height_f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // RENDER OPENGL HERE
        // Draw Horizon Block Top
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HORIZON_BLOCK_TOP_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Horizon_block_top);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Horizon Block Bottom
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HORIZON_BLOCK_BOTTOM_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Horizon_block_bottom);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Horizon Line
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HORIZON_LINE_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Horizon_block_line);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Top Box 1
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(TOP_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Top_box_1);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Top Box 2
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(TOP_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Top_box_2);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Bottom Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(TOP_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Bottom_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Altitude Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Altitude_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Altitude Select Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Altitude_select_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
         
        // Draw Altitude Current Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Altitude_current_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);

        // Draw Altitude Set Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Altitude_set_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Vertical Speed Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Vertical_speed_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Airspeed Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Airspeed_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw OAT Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Oat_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Airspeed Current Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Airspeed_current_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Airspeed TAS Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Airspeed_tas_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Chevron
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(CHEVRON_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Chevron);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 7);
        glDisableClientState(GL_VERTEX_ARRAY);
        /*
        // Draw Chevron Left Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(CHEVRON_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Chevron_left_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Chevron Left Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(CHEVRON_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Chevron_right_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw DG
        glColor3ub(DG_COLOR);
        drawFilledCircle(dg_center_x, dg_center_y, dg_radius);
        
        // Draw Current Heading Box
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(HIGHLIGHT_BOX_COLOR);
        glVertexPointer(3, GL_FLOAT, 0, Current_heading_box);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Current Heading Triangle
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3ub(WHITE);
        glVertexPointer(3, GL_FLOAT, 0, Current_heading_triangle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw Turn Coordinator Arc
        glColor3ub(WHITE);
        DrawArc(center_x, horizon_origin, float(main_box_height / 2 + highlight_box_height), PI / 6, 2 * PI / 3, 40);
        */
        // Swap Front and Back Buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        std::cout<<"Left Arrow"<<std::endl;
    }
    std::cout<<key<<std::endl;
}
