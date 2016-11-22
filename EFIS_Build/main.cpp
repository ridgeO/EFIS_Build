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
// Load Maps
#include <map>
// Load Shader Header
#include "shader.hpp"
// Load GLM for additional math and rotations
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Load GLFW & GLEW from custom header
#include "glfw_app.hpp"
// Load GLFW & GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>
//#include "GLFW/glfw3.h"
// Load FreeType for text processing
#include <ft2build.h>
#include FT_FREETYPE_H

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
    //Generate notches for dg
    glColor3b(255,255,255);
    glBegin(GL_LINES);
    for(int bn = 0; bn < 36; bn++){
        glVertex3f(x,y,0); //center of circle
        glVertex3f(x + (radius * cos(bn)),
                   y + (radius * sin(bn)),0
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

//-------------------------------------
// Set commonly used heights and widths
//-------------------------------------
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
float horizon_block_leftbound = left_bound - .5 * horizon_block_width;
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
float chevron_mid_thickness = chevron_height * .2;
float dg_radius = screen_height_f * .21;
float dg_center_x = center_x;
float dg_center_y = bottom_bound + bounding_box_height + dg_radius;
float current_heading_box_bottombound = dg_center_y + dg_radius;
float current_heading_box_topbound = current_heading_box_bottombound + highlight_box_height;
float current_heading_box_leftbound = center_x - dg_radius / 4;
float current_heading_box_rightbound = center_x + dg_radius / 4;
float current_heading_triangle_leftbound = center_x - dg_radius / 16;
float current_heading_triangle_rightbound = center_x + dg_radius / 16;
float current_heading_triangle_bottombound = current_heading_box_bottombound - sqrtf((dg_radius/8)*(dg_radius/8)-(dg_radius/16)*(dg_radius/16));

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
    float(chevron_leftbound - .5 * chevron_width),chevron_topbound,0,
    float(chevron_leftbound - .25 * chevron_width),chevron_topbound,0,
    float(chevron_leftbound - .25 * chevron_width),float(chevron_topbound - chevron_height * .25),0,
    float(chevron_leftbound - .5 * chevron_width),float(chevron_topbound - chevron_height * .25),0
};

// Set points for Chevron_right_box
GLfloat Chevron_right_box[] =
{
    float(chevron_rightbound + .5 * chevron_width),chevron_topbound,0,
    float(chevron_rightbound + .25 * chevron_width),chevron_topbound,0,
    float(chevron_rightbound + .25 * chevron_width),float(chevron_topbound - chevron_height * .25),0,
    float(chevron_rightbound + .5 * chevron_width),float(chevron_topbound - chevron_height * .25),0
};

//------------------------------
// HEADING INDICATOR
//------------------------------

// Set points for Current_heading_box
GLfloat Current_heading_box[] =
{
    current_heading_box_leftbound,current_heading_box_topbound,0,
    current_heading_box_rightbound,current_heading_box_topbound,0,
    current_heading_box_rightbound,current_heading_box_bottombound,0,
    current_heading_box_leftbound,current_heading_box_bottombound,0
};

// Set points for Current_heading_triangle
GLfloat Current_heading_triangle[] =
{
    current_heading_triangle_leftbound,current_heading_box_bottombound,0,
    current_heading_triangle_rightbound,current_heading_box_bottombound,0,
    center_x,current_heading_triangle_bottombound,0
};

//-------------------------------
// Define Movement Variables
//-------------------------------

float horizon_blocks_y = 0;
float horizon_blocks_rot = 0;
float dg_rot = 0;
float turn_coordinator_rot = 0;

//-------------------------------
// Define Freetype Relevant data
//-------------------------------
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

//-------------------------------
// Begin main function
//-------------------------------

int main (void)
{
    // Initialize GLFW
    GLFWwindow *window;
    
    // Make sure library initializes and close out GLFW if not
    if (!glfwInit())
    {
        return -1;
    }
   /* glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
   */
    // Set initial openGL window with dimensions and title
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "G1000 Build V4", NULL, NULL);
    
    // Set KeyCallback Function
    //glfwSetKeyCallback(window, keyCallback);
    
    // Make sure window initializes and close out OpenGL if not
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set OpenGL options
    /* glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */
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
    
    // Setup Freetype
    // Compile and setup the shader
    Shader shader("/Users/rto/Desktop/EFIS_Build/EFIS_Build/shaders/text.vs", "/Users/rto/Desktop/EFIS_Build/EFIS_Build/shaders/text.frag");
    glm::mat4 projection = glm::ortho(0.0f, screen_width_f, 0.0f, screen_height_f);
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // FreeType
    FT_Library ft;
    
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    
    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "/library/fonts/arial.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        };
        
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                     GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer
                     );
        
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    };
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
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
        
        glPushMatrix();
        glTranslatef(horizon_block_width/4,horizon_origin+horizon_blocks_y,0);
        glRotatef(horizon_blocks_rot,0.f,0.f,1.f);
        glTranslatef(-horizon_block_width/4,-horizon_origin,0);
        
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
        
        glPopMatrix();
        
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
        
        // Draw Turn Coordinator Arc
        glPushMatrix();
        glTranslatef(center_x,horizon_origin,0);
        glRotatef(turn_coordinator_rot, 0, 0, 1);
        glTranslatef(-center_x,-horizon_origin,0);
        glColor3ub(WHITE);
        DrawArc(center_x, horizon_origin, float(main_box_height / 2 + highlight_box_height), PI / 6, 2 * PI / 3, 40);
        glPopMatrix();
        
        // Draw DG
        glPushMatrix();
        glTranslatef(dg_center_x,dg_center_y,0);
        glRotatef(dg_rot, 0, 0, 1);
        glTranslatef(-dg_center_x,-dg_center_y,0);
        glColor3ub(DG_COLOR);
        drawFilledCircle(dg_center_x,dg_center_y, dg_radius);
        glPopMatrix();
        
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
        
        // Clear the colorbuffer
        glColor3ub(WHITE);
        RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        
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
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
    {
        std::cout<<"Left Arrow"<<std::endl;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
    {
        horizon_blocks_y += 2;
    }
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
    {
        horizon_blocks_y -= 2;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
    {
        horizon_blocks_rot += 1;
        dg_rot += 1;
        turn_coordinator_rot += 1;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
    {
        horizon_blocks_rot -= 1;
        dg_rot -= 1;
        turn_coordinator_rot -= 1;
    }
    std::cout<<key<<std::endl;
}

//-------------------------------
// Setup Freetype and Render Text Function
//-------------------------------

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    shader.Use();
    glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    
    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        
        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
