#ifndef GRAPHICS_FA_H
#define GRAPHICS_FA_H
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdint.h>
#include "glext.h"
#include "log_FA.h"

#define USE_EBO_TO_DRAW_QUAD 1
#define V_SYNC 0 

//NOTE(Stanisz13): TEXTURES:
PFNGLGENERATEMIPMAPPROC glGenerateMipmap_FA;
PFNGLACTIVETEXTUREPROC glActiveTexture_FA;

//NOTE(Stanisz13): FRAMEBUFFERS
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers_FA;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer_FA;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D_FA;

//NOTE(Stanisz13): SHADERS
PFNGLCREATESHADERPROC glCreateShader_FA;
PFNGLSHADERSOURCEPROC glShaderSource_FA;
PFNGLCOMPILESHADERPROC glCompileShader_FA;
PFNGLGETSHADERIVPROC glGetShaderiv_FA;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog_FA;
PFNGLATTACHSHADERPROC glAttachShader_FA;
PFNGLDELETESHADERPROC glDeleteShader_FA;

//NOTE(Stanisz13): UNIFORMS
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_FA;
PFNGLUNIFORM1FPROC glUniform1f_FA;
PFNGLUNIFORM2FPROC glUniform2f_FA;
PFNGLUNIFORM2FVPROC glUniform1fv_FA;
PFNGLUNIFORM2FVPROC glUniform2fv_FA;
PFNGLUNIFORM1UIPROC glUniform1ui_FA;
PFNGLUNIFORM1UIVPROC glUniform1uiv_FA;
PFNGLUNIFORM1IPROC glUniform1i_FA;
PFNGLUNIFORM1IVPROC glUniform1iv_FA;
PFNGLUNIFORM3FPROC glUniform3f_FA;
PFNGLUNIFORM3FVPROC glUniform3fv_FA;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv_FA;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv_FA;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv_FA;

//NOTE(Stanisz13): PROGRAMS
PFNGLCREATEPROGRAMPROC glCreateProgram_FA;
PFNGLLINKPROGRAMPROC glLinkProgram_FA;
PFNGLGETPROGRAMIVPROC glGetProgramiv_FA;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog_FA;
PFNGLUSEPROGRAMPROC glUseProgram_FA;
PFNGLDELETEPROGRAMPROC glDeleteProgram_FA;

//NOTE(Stanisz13): BUFFERS
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_FA;
PFNGLGENBUFFERSPROC glGenBuffers_FA;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_FA;
PFNGLBINDBUFFERPROC glBindBuffer_FA;
PFNGLBUFFERDATAPROC glBufferData_FA;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_FA;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_FA;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray_FA;
PFNGLDELETEBUFFERSPROC glDeleteBuffers_FA;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays_FA;

//NOTE(Stanisz13): V_SYNC
PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA_FA;
PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT_FA;
PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI_FA;

typedef struct
{
    unsigned minimalGLXVersionMajor;
    unsigned minimalGLXVersionMinor;
    unsigned minimalGLVersionMajor;
    unsigned minimalGLVersionMinor;
    unsigned windowWidth;
    unsigned windowHeight;
    char* name;
    
    Display* display;
    Window window;
    Colormap cmap;
    GLXContext ctx;

    Atom deleteMessage;
    
} ContextData;

typedef struct
{
    unsigned mask;

} UserVSyncData;

typedef struct
{
    unsigned basicProgram;
    unsigned texture;
    unsigned VAO;
    unsigned* pixels;
    
} PixelBufferData;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    
} Color;

typedef struct
{
    unsigned fbo[2];
    unsigned texture[2];

} PingpongBuffer;

typedef struct
{
    unsigned VBO, VAO, EBO;
    
} ScreenQuadWithEBO;

typedef struct
{
    unsigned VBO, VAO;
    
} ScreenQuad;

typedef struct
{
    unsigned left;
    unsigned right;
    unsigned middle;

    //NOTE(Stanisz13): Left top corner is (0, 0), bottom right corner is (screenW, screenH);
    float posX;
    float posY;
    
    int wheel; //NOTE(Stanisz13): sign depending on scroll direction

} MouseState;

//NOTE(Stanisz13): GLOBALS
MouseState mouseState_FA;
unsigned keysPressed_FA[200];
ContextData contextData_FA;
UserVSyncData vSyncData_FA;

void configureOpenGL();

void freeContextData();

void loadFunctionPointers();

//NOTE(Stanisz13): Memory layout of mask is expected to be ABGR.
Color unsignedToColor(unsigned mask);

//NOTE(Stanisz13): The packed channels are in ABGR memory layout.
unsigned colorToUnsigned(const Color* c);

Color RGBAtoColor(unsigned char r, unsigned char g,
                  unsigned char b, unsigned char a);

void createTextureForDrawingBuffer(PixelBufferData* pdata);

void drawTextureWithBufferData(PixelBufferData* pdata);

void freePixelData(PixelBufferData* pdata);

float lerpFloat(float v0, float v1, float t);

Color lerpColor(const Color* a, const Color* b, const float t);

void configurePingpongBuffer(PingpongBuffer* pbuf);

void configureScreenQuadWithEBO(ScreenQuadWithEBO* squad);

void configureScreenQuad(ScreenQuad* squad);

void freeScreenQuad(ScreenQuad* squad);

void freeScreenQuadWithEBO(ScreenQuadWithEBO* squad);

unsigned createShaderProgram(const char* pathToVS, const char* pathToFS);

void enableVSyncIfPossible();

void disableVSyncIfPossible();

void enableAdaptiveVSyncIfPossible();

//NOTE(Stanisz13): The bmp must have alpha channel, values for pixels are
// expected to be 32bit. The memory layout of loaded image is ARGB, but
// the memory layout of stored data is ABGR.
unsigned loadBMPtexture(const char* filename, unsigned* texture);

void mouseCoordsToNDC();

#endif
