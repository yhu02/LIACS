///////////////////////////////////////////////////////////////////////////////
// Name: Yvo Hu
// Student number: 2962802
// Date: December 2022
// Title: Fireworks
// Original parts:
// Creating a firework shape
// Playing explosion sound
// Firework mechanics
// Explosion particles
// Textures
// Sounds
// Credits to the people down below for allowing me to compose this program using their work
///////////////////////////////////////////////////////////////////////////////
// main.cpp
// Bmp.cpp
// Cylinder.cpp
// Texture loader
// 
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-02
// UPDATED: 2020-01-21
// http://www.songho.ca/opengl/gl_cylinder.html
///////////////////////////////////////////////////////////////////////////////
// Audio SDL2
// https://github.com/jarreed0/random_projects/tree/main/c%2B%2B/mixer
// ///////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <chrono>
#include "Bmp.h"
#include "Cylinder.h"

#define N_SOUNDS 4
#define N_TEXTURES 8
#define MAXPARTICLES 1000

// https://stackoverflow.com/questions/4310277/producing-random-float-from-negative-to-positive-range
float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

void initGL();
int initGLUT(int argc, char **argv);
bool initSharedMem();
void initLights();
void toPerspective();
void drawOneParticle();
GLuint loadTexture(const char *fileName, bool wrap = true);

int loadSound(const char *fileName);
int playSound(int s);

class Firework;
class pinfo;

// constants
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 500;
const float CAMERA_DISTANCE = 4.0f;
const int TEXT_WIDTH = 8;
const int TEXT_HEIGHT = 13;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraAngleZ;
int drawMode;
int imageWidth;
int imageHeight;

// Texture ids
std::vector<GLuint> coneIds;
std::vector<GLuint> cylinderIds;
// Sound ids, explosionsounds and sounds are a subset of sounds
std::vector<size_t> explosionSounds;
std::vector<size_t> flightSounds;
std::vector<Mix_Chunk *> sounds;
int volume;

// cylinder: min sectors = 3, min stacks = 1
Cylinder cylinder(0.8f, 0.8f, 5.0f, 36, 8, false); // baseRadius, topRadius, height, sectors, stacks, flat shading
Cylinder cone(1.5f, 0.0f, 2.0f, 36, 8, true);      // baseRadius, topRadius, height, sectors, stacks, smooth(default)

float g_orientation = 15.0; // y axis
bool gravityEnabled = false;

// Global vector containing all firework instances
std::vector<Firework> fireworksArr;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Particle class
class pinfo
{
public:
    float width;
    float x, y, z;
    float v_x, v_y, v_z;
    float r, g, b;
    bool enabled;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Firework class
class Firework
{
public:
    std::vector<pinfo> particleArr;
    double conePosition[3];
    double cylinderPosition[3];
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    bool explosion;
    GLuint cylinderId;
    GLuint coneId;
    Firework()
    {
        explosion = false;
        begin = std::chrono::steady_clock::now();
        // Spawn area
        float mul = 100;
        float x = RandomNumber(-mul, mul);
        float z = RandomNumber(-mul, mul);

        conePosition[0] = x;
        conePosition[1] = 3;
        conePosition[2] = z;
        cylinderPosition[0] = x;
        cylinderPosition[1] = 0;
        cylinderPosition[2] = z;

        cylinderId = cylinderIds[(int)RandomNumber(0, cylinderIds.size())];
        coneId = coneIds[(int)RandomNumber(0, coneIds.size())];

        playFlightSound();
    }
    // Pick a random explosion sound
    void playExplosionSound()
    {
        playSound(explosionSounds[(int)RandomNumber(0, explosionSounds.size())]);
    }
    // Pick a random flight sound
    void playFlightSound()
    {
        playSound(flightSounds[(int)RandomNumber(0, flightSounds.size())]);
    }

    void initParticles()
    {
        unsigned int i;
        float r = 1.0 * (rand() / (float)RAND_MAX);
        float g = 1.0 * (rand() / (float)RAND_MAX);
        float b = 1.0 * (rand() / (float)RAND_MAX);

        float mult = RandomNumber(0.5f, 10.0f);
        int choice = (int)RandomNumber(0, 100) % 2;

        for (i = 0; i < MAXPARTICLES; i = i + 1)
        {
            float v_x = RandomNumber(-1, 1) * mult;
            float v_y = RandomNumber(-1, 1) * mult;
            float v_z = RandomNumber(-1, 1) * mult;

            pinfo particleSet;

            // Disable some particles, and make a sphere
            if (choice == 1)
            {
                if (sqrt(v_x * v_x + v_y * v_y + v_z * v_z) > mult)
                {
                    particleSet.enabled = false;
                    particleArr.push_back(particleSet);
                    continue;
                }
            }
            particleSet.enabled = true;

            particleSet.width = 3.0 * (rand() / (float)RAND_MAX) + 1.0;
            particleSet.x = conePosition[0];
            particleSet.y = conePosition[1];
            particleSet.z = conePosition[2];

            particleSet.v_x = v_x;
            particleSet.v_y = v_y;
            particleSet.v_z = v_z;

            particleSet.r = r;
            particleSet.g = g;
            particleSet.b = b;

            particleArr.push_back(particleSet);
        }
    }

    void drawFirework()
    {

        // draw cylinder
        glPushMatrix();
        glTranslatef(cylinderPosition[0], cylinderPosition[1], cylinderPosition[2]);
        glRotatef(-90, 1, 0, 0);
        // rainbow
        if (cylinderId == cylinderIds[cylinderIds.size() - 1])
        {
            glBindTexture(GL_TEXTURE_2D, cylinderIds[(int)RandomNumber(0, cylinderIds.size())]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, cylinderId);
        }
        glColor3f(1.f, 1.f, 1.f);
        cylinder.draw();
        glPopMatrix();

        // draw cone
        glPushMatrix();
        glTranslatef(conePosition[0], conePosition[1], conePosition[2]);
        glRotatef(-90, 1, 0, 0);
        // rainbow
        if (coneId == coneIds[coneIds.size() - 1])
        {
            glBindTexture(GL_TEXTURE_2D, coneIds[(int)RandomNumber(0, coneIds.size())]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, coneId);
        }
        cone.draw();
        glPopMatrix();
    }

    // draw explosion
    void drawExplosion()
    {
        // only draw the vertices in triangles
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        for (unsigned it = 0; it < particleArr.size(); it++)
        {
            if (particleArr[it].enabled)
            {
                glPushMatrix();
                glTranslatef(particleArr[it].x, particleArr[it].y, particleArr[it].z);
                glScalef(particleArr[it].width, particleArr[it].width, particleArr[it].width);
                glColor3f(particleArr[it].r, particleArr[it].g, particleArr[it].b);
                drawOneParticle();
                glPopMatrix();
            }
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

private:
};

void drawOneParticle()
{
    glBegin(GL_TRIANGLE_STRIP);
    // triangle 1
    glVertex3f(-0.5, 0.0, 0.5); // A
    glVertex3f(0.0, 0.0, -0.5); // B
    glVertex3f(0.0, 1.0, 0.0);  // top
    // triangle 2
    glVertex3f(0.5, 0.0, 0.5); // C
    // triangle 3
    glVertex3f(-0.5, 0.0, 0.5); // A again
    // triangle 4 (bottom)
    glVertex3f(0.0, 0.0, -0.5); // B again
    glEnd();
}

void timerCB(int value)
{
    static int lastTime;
    int thisTime;
    float time;
    thisTime = glutGet(GLUT_ELAPSED_TIME);
    time = (thisTime - lastTime) / 500.0;
    lastTime = thisTime;

    for (int i = fireworksArr.size() - 1; i >= 0; i--)
    {
        fireworksArr[i].conePosition[1] += 0.5;
        fireworksArr[i].cylinderPosition[1] += 0.5;
        fireworksArr[i].end = std::chrono::steady_clock::now();
        for (unsigned int it = 0; it < fireworksArr[i].particleArr.size(); it++)
        {
            fireworksArr[i].particleArr[it].x = fireworksArr[i].particleArr[it].x + fireworksArr[i].particleArr[it].v_x * time;
            fireworksArr[i].particleArr[it].y = fireworksArr[i].particleArr[it].y + fireworksArr[i].particleArr[it].v_y * time;
            fireworksArr[i].particleArr[it].z = fireworksArr[i].particleArr[it].z + fireworksArr[i].particleArr[it].v_z * time;

            if (gravityEnabled)
            {
                if (fireworksArr[i].particleArr[it].y > 0)
                {
                    fireworksArr[i].particleArr[it].v_y = fireworksArr[i].particleArr[it].v_y - 9.81 * time * 0.1f;
                }
                else if (fireworksArr[i].particleArr[it].y <= 0)
                {
                    fireworksArr[i].particleArr[it].v_y = 0;
                    fireworksArr[i].particleArr[it].y = 0;
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timerCB, 30);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUDIO
int loadSound(const char *filename)
{
    Mix_Chunk *m = NULL;
    m = Mix_LoadWAV(filename);
    if (m == NULL)
    {
        printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
        return -1;
    }
    sounds.push_back(m);
    return sounds.size() - 1;
}

void setVolume(int v)
{
    volume = (MIX_MAX_VOLUME * v) / 100;
}

int playSound(int s)
{
    Mix_PlayChannel(-1, sounds[s], 0);
    return 0;
}

int initMixer()
{
    Mix_Init(MIX_INIT_MP3);
    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_Mixer couldnt init. Err: %s\n", Mix_GetError());
        return -1;
    }
    setVolume(80);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    srand(time(NULL));
    initMixer();
    Mix_Volume(1, volume);

    // init global vars
    initSharedMem();

    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();

    // init firework textures
    for (size_t i = 0; i < N_SOUNDS; i++)
    {
        std::string explosionPath = "sound/explosion" + std::to_string(i) + ".wav";
        std::string flightPath = "sound/flight" + std::to_string(i) + ".wav";

        explosionSounds.push_back(loadSound(explosionPath.c_str()));
        flightSounds.push_back(loadSound(flightPath.c_str()));
    }
    // init firework textures
    for (size_t i = 0; i < N_TEXTURES; i++)
    {
        std::string conePath = "img/cone" + std::to_string(i) + ".bmp";
        std::string cylinderPath = "img/cylinder" + std::to_string(i) + ".bmp";

        coneIds.push_back(loadTexture(conePath.c_str(), true));
        cylinderIds.push_back(loadTexture(cylinderPath.c_str(), true));
    }

    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    glutMainLoop(); /* Start GLUT event-processing loop */

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); // display mode

    glutInitWindowSize(screenWidth, screenHeight); // window size

    glutInitWindowPosition(100, 100); // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    int handle = glutCreateWindow("Fireworks"); // param is the title of window

    // register GLUT callback functions

    glutReshapeFunc(reshapeCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    glutDisplayFunc(displayCB);
    glutKeyboardFunc(keyboardCB);
    glutTimerFunc(30, timerCB, 30); // redraw only every given millisec

    return handle;
}

///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
    glShadeModel(GL_SMOOTH);               // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0); // background color
    glClearStencil(0);        // clear stencil buffer
    glClearDepth(1.0f);       // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();
}

///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = mouseMiddleDown = false;
    mouseX = mouseY = 0;

    cameraAngleX = cameraAngleY = 0.0f;
    cameraAngleZ = CAMERA_DISTANCE;

    drawMode = 0; // 0:fill, 1: wireframe, 2:points

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.5f, .5f, .5f, 1.0f}; // ambient light
    GLfloat lightKd[] = {.8f, .8f, .8f, 1.0f}; // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};          // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0); // MUST enable each light source after configuration
}

///////////////////////////////////////////////////////////////////////////////
// load raw image as a texture
///////////////////////////////////////////////////////////////////////////////
GLuint loadTexture(const char *fileName, bool wrap)
{
    Image::Bmp bmp;
    if (!bmp.read(fileName))
    {
        return 0; // exit if failed load image
    }

    // get bmp info
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    const unsigned char *data = bmp.getDataRGB();

    // We assume the image is 8-bit, 24-bit or 32-bit BMP
    GLenum format;
    int bpp = bmp.getBitCount();
    if (bpp == 8)
        format = GL_LUMINANCE;
    else if (bpp == 24)
        format = GL_RGB;
    else if (bpp == 32)
        format = GL_RGBA;
    else
        return 0; // NOT supported, exit

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // copy texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // build our texture mipmaps
    switch (bpp)
    {
    case 8:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
        break;
    case 24:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case 32:
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    }

    return texture;
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // set material
    float ambient[] = {0.5f, 0.5f, 0.5f, 1};
    float diffuse[] = {0.8f, 0.8f, 0.8f, 1};
    float specular[] = {1.0f, 1.0f, 1.0f, 1};
    float shininess = 128;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // left,right; up,down; depth
    glTranslatef(-cameraAngleY, -cameraAngleX - 10, -cameraAngleZ - 100);
    glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.5f, 0.5f, 0.5f);

    // base platform
    glBegin(GL_QUADS);
    glVertex3f(-400.0, 0.0, -400.0);
    glVertex3f(-400.0, 0.0, 400.0);
    glVertex3f(400.0, 0.0, 400.0);
    glVertex3f(400.0, 0.0, -400.0);
    glEnd();

    // Explosion
    for (int i = fireworksArr.size() - 1; i >= 0; i--)
    {
        // Draw explosion
        if (fireworksArr[i].explosion)
        {
            fireworksArr[i].end = std::chrono::steady_clock::now();
            fireworksArr[i].drawExplosion();
            int difference = (int)std::chrono::duration_cast<std::chrono::seconds>(fireworksArr[i].end - fireworksArr[i].begin).count();
            if (difference > RandomNumber(3, 10))
            {
                fireworksArr.erase(std::next(fireworksArr.begin(), i), std::next(fireworksArr.begin(), i + 1));
                glPopMatrix();

                return;
            }
        } // Chance for firework to randomly explode
        else if (fireworksArr[i].conePosition[1] >= RandomNumber(0, 1000))
        {

            fireworksArr[i].playExplosionSound();
            fireworksArr[i].initParticles();

            fireworksArr[i].explosion = true;
        }
    }
    // Draw unexploded fireworks
    for (unsigned int i = 0; i < fireworksArr.size(); i++)
    {
        if (fireworksArr[i].explosion == false)
        {
            fireworksArr[i].drawFirework();
        }
    }
    glPopMatrix();

    // Reset colors
    glColor3f(1.f, 1.f, 1.f);
    glBindTexture(GL_TEXTURE_2D, 0);
    glutSwapBuffers();
}

void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    toPerspective();
}

void keyboardCB(unsigned char key, int x, int y)
{

    switch (key)
    {
    case 27: // ESCAPE
        exit(0);
        break;

    case ' ':
    {
        int count = cylinder.getSectorCount();
        if (count < 36)
            count += 4;
        else
            count = 4;
        cylinder.setSectorCount(count);
        cylinder.setStackCount(count / 4);
        cone.setSectorCount(count);
        cone.setStackCount(count / 4);
        break;
    }
    case 'a': // left
        cameraAngleY = cameraAngleY - 10.0;
        break;
    case 'd': // right
        cameraAngleY = cameraAngleY + 10.0;
        break;
    case 'h': // home
        cameraAngleX = 0.0f;
        cameraAngleY = 0.0f;
        cameraAngleZ = 0.0f;
        g_orientation = 15.0;
        break;
    case 'f': // fireworks
        fireworksArr.push_back(Firework{});
        break;
    case 'g': // toggle gravity
        gravityEnabled = !gravityEnabled;
        break;
    default:;
    }
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if (state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if (state == GLUT_UP)
            mouseRightDown = false;
    }

    else if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if (state == GLUT_UP)
            mouseMiddleDown = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if (mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX -= (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if (mouseRightDown)
    {
        cameraAngleZ -= (y - mouseY) * 0.2f;
        mouseY = y;
    }
}
