#include "graphics_FA.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092

typedef GLXContext (*glXCreateContextAttribsARBProc)
(Display*, GLXFBConfig, GLXContext, Bool, const int*);

unsigned isExtensionSupported(const char *extList, const char *extension)
{
    //copied from :
    // https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
    // ---
    // Helper to check for extension string presence.  Adapted from:
    //   http://www.opengl.org/resources/features/OGLextensions/

    const char *start;
    const char *where, *terminator;

    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return 0;

    for (start=extList;;)
    {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return 1;

        start = terminator;
    }

    return 0;
}

int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
{
    exit(0);
    return 0;
}

typedef enum
{
    MESA_V_SYNC = 1,
    EXT_V_SYNC = 2,
    SGI_V_SYNC = 4

} UserVSyncDataMasks;

void configureOpenGL()
{
    // Open a comminication to X server with default screen name.
    contextData_FA.display = XOpenDisplay(NULL);

#if 0
    XSynchronize(contextData_FA.display, 0);
#else
    XSynchronize(contextData_FA.display, 1);
#endif

    if (!contextData_FA.display)
    {
        logError("Unable to start communication with X server!");
        exit(0);
    }

    // Get a matching framebuffer config
     int visual_attribs[] =
        {
            GLX_X_RENDERABLE    , 1,
            GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
            GLX_RENDER_TYPE     , GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
            GLX_RED_SIZE        , 8,
            GLX_GREEN_SIZE      , 8,
            GLX_BLUE_SIZE       , 8,
            GLX_ALPHA_SIZE      , 8,
            GLX_DEPTH_SIZE      , 24,
            GLX_STENCIL_SIZE    , 8,
            GLX_DOUBLEBUFFER    , 1,
            //GLX_SAMPLE_BUFFERS  , 1,
            //GLX_SAMPLES         , 4,
            None
        };

    int glx_major, glx_minor;

    // Check if the version is not less than minimal
    if (!glXQueryVersion(contextData_FA.display, &glx_major, &glx_minor))
    {
        logError("Could not obtain GLX drivers version!");
        exit(0);
    }

    printf("Your version of GLX drivers is: %u.%u\n", glx_major, glx_minor);

    if (glx_major < contextData_FA.minimalGLXVersionMajor
        || glx_minor < contextData_FA.minimalGLXVersionMinor)
    {
        logError("Your version of GLX drivers does not match the minimum requirements!");
        exit(0);
    }

    int fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(contextData_FA.display, DefaultScreen(contextData_FA.display),
                                         visual_attribs, &fbcount);
    if (!fbc)
    {
        logError("Failed to retrieve any framebuffer config");
        exit(0);
    }

#if 0
    printf("Found %d matching framebuffer configs\n", fbcount);
#endif

    // Pick the framebuffer config/visual with the most samples per pixel
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
    int i;

    for (i=0; i<fbcount; ++i)
    {
        XVisualInfo *vi = glXGetVisualFromFBConfig(contextData_FA.display, fbc[i]);
        if (vi)
        {
            int samp_buf, samples;
            glXGetFBConfigAttrib(contextData_FA.display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
            glXGetFBConfigAttrib(contextData_FA.display, fbc[i], GLX_SAMPLES, &samples);
#if 0
            printf("  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
                   " SAMPLES = %d\n",
                   i, vi->visualid, samp_buf, samples);
#endif
            if (best_fbc < 0 || (samp_buf && samples > best_num_samp))
                best_fbc = i, best_num_samp = samples;
            if (worst_fbc < 0 || (!samp_buf || samples < worst_num_samp))
                worst_fbc = i, worst_num_samp = samples;
        }

        XFree(vi);
    }

    GLXFBConfig bestFbc = fbc[best_fbc];

    XFree(fbc);

    XVisualInfo *vi = glXGetVisualFromFBConfig(contextData_FA.display, bestFbc);

#if 0
    printf("Chosen visual ID = 0x%x\n", vi->visualid);
#endif

    XSetWindowAttributes swa;
    swa.colormap = contextData_FA.cmap = XCreateColormap(contextData_FA.display,
                                           RootWindow(contextData_FA.display, vi->screen),
                                           vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    contextData_FA.window = XCreateWindow(contextData_FA.display, RootWindow(contextData_FA.display, vi->screen),
                                0, 0, contextData_FA.windowWidth, contextData_FA.windowHeight,
                                0, vi->depth, InputOutput,
                                vi->visual,
                                CWBorderPixel|CWColormap|CWEventMask, &swa);
    if (!contextData_FA.window)
    {
        logError("Failed to create X window!");
        exit(0);
    }

    XFree(vi);

    XStoreName(contextData_FA.display, contextData_FA.window, contextData_FA.name);

    XMapWindow(contextData_FA.display, contextData_FA.window);

    const char *glxExts = glXQueryExtensionsString(contextData_FA.display,
                                                   DefaultScreen(contextData_FA.display));

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
        glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

    int (*oldHandler)(Display*, XErrorEvent*) =
        XSetErrorHandler(&ctxErrorHandler);

    //logS(glxExts);

    // Check for the GLX_ARB_create_context extension string and the function.
    if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") ||
         !glXCreateContextAttribsARB)
    {
        logError("glXCreateContextAttribsARB() not found!");
        exit(0);
    }
    else
    {
        if (isExtensionSupported(glxExts, "GLX_MESA_swap_control"))
        {
            vSyncData_FA.mask |= MESA_V_SYNC;
        }
        if (isExtensionSupported(glxExts, "GLX_EXT_swap_control_tear"))
        {
            vSyncData_FA.mask |= EXT_V_SYNC;
        }
        if (isExtensionSupported(glxExts, "GLX_SGI_swap_control"))
        {
            vSyncData_FA.mask |= SGI_V_SYNC;
        }


        int context_attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, contextData_FA.minimalGLVersionMajor,
                GLX_CONTEXT_MINOR_VERSION_ARB, contextData_FA.minimalGLVersionMinor,
                //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                None
            };

        contextData_FA.ctx = glXCreateContextAttribsARB(contextData_FA.display, bestFbc, 0,
                                          1, context_attribs);

        // Sync to ensure any errors generated are processed.
        XSync(contextData_FA.display, False);

        if (contextData_FA.ctx)
            printf("Created GL %u.%u context\n", contextData_FA.minimalGLVersionMajor, contextData_FA.minimalGLVersionMinor);
        else
        {
            logError("Failed to create GL %u.%u context!");
            exit(0);
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync(contextData_FA.display, 0);
    
    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (!contextData_FA.ctx)
    {
        logError("Failed to create an OpenGL context!");
        exit(0);
    }

#if 0
    // Verifying that context is a direct context
    if (!glXIsDirect(contextData_FA.display, contextData_FA.ctx))
    {
        logS("Indirect GLX rendering context obtained");
    }
    else
    {
        logS("Direct GLX rendering context obtained");
    }
#endif

    contextData_FA.deleteMessage = XInternAtom(contextData_FA.display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(contextData_FA.display, contextData_FA.window, &contextData_FA.deleteMessage, 1);

    XSelectInput(contextData_FA.display, contextData_FA.window, ButtonPressMask|ButtonReleaseMask|
                 StructureNotifyMask|KeyPressMask|KeyReleaseMask|KeymapStateMask|
                 PointerMotionMask);
    
    glXMakeCurrent(contextData_FA.display, contextData_FA.window, contextData_FA.ctx);
}

void freeContextData(ContextData* cdata)
{
    glXMakeCurrent(contextData_FA.display, 0, 0);
    glXDestroyContext(contextData_FA.display, contextData_FA.ctx);

    XDestroyWindow(contextData_FA.display, contextData_FA.window);
    XFreeColormap(contextData_FA.display, contextData_FA.cmap);
    XCloseDisplay(contextData_FA.display);
}

void loadFunctionPointers()
{
    //NOTE(Stanisz13): TEXTURES:
    glGenerateMipmap_FA = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddressARB((const unsigned char*)"glGenerateMipmap");
    glActiveTexture_FA = (PFNGLACTIVETEXTUREPROC)glXGetProcAddressARB((const unsigned char*)"glActiveTexture");

    //NOTE(Stanisz13): FRAMEBUFFERS
    glGenFramebuffers_FA = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glGenFramebuffers");
    glBindFramebuffer_FA = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress((const unsigned char*)"glBindFramebuffer");
    glFramebufferTexture2D_FA = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress((const unsigned char*)"glFramebufferTexture2D");

    //NOTE(Stanisz13): SHADERS
    glCreateShader_FA = (PFNGLCREATESHADERPROC)glXGetProcAddress((const unsigned char*)"glCreateShader");
    glShaderSource_FA = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const unsigned char*)"glShaderSource");
    glCompileShader_FA = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const unsigned char*)"glCompileShader");
    glGetShaderiv_FA = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const unsigned char*)"glGetShaderiv");
    glGetShaderInfoLog_FA = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const unsigned char*)"glGetShaderInfoLog");
    glAttachShader_FA = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const unsigned char*)"glAttachShader");
    glDeleteShader_FA = (PFNGLDELETESHADERPROC)glXGetProcAddress((const unsigned char*)"glDeleteShader");

    //NOTE(Stanisz13): UNIFORMS
    glGetUniformLocation_FA = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const unsigned char*)"glGetUniformLocation");
    glUniform1f_FA = (PFNGLUNIFORM1FPROC)glXGetProcAddress((const unsigned char*)"glUniform1f");
    glUniform2f_FA = (PFNGLUNIFORM2FPROC)glXGetProcAddress((const unsigned char*)"glUniform2f");
    glUniform1fv_FA = (PFNGLUNIFORM2FVPROC)glXGetProcAddress((const unsigned char*)"glUniform1fv");
    glUniform2fv_FA = (PFNGLUNIFORM2FVPROC)glXGetProcAddress((const unsigned char*)"glUniform2fv");
    glUniform1ui_FA = (PFNGLUNIFORM1UIPROC)glXGetProcAddress((const unsigned char*)"glUniform1ui");
    glUniform1uiv_FA = (PFNGLUNIFORM1UIVPROC)glXGetProcAddress((const unsigned char*)"glUniform1uiv");
    glUniform1i_FA = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const unsigned char*)"glUniform1i");
    glUniform1iv_FA = (PFNGLUNIFORM1IVPROC)glXGetProcAddress((const unsigned char*)"glUniform1iv");
    glUniform3f_FA = (PFNGLUNIFORM3FPROC)glXGetProcAddress((const unsigned char*)"glUniform3f");
    glUniform3fv_FA = (PFNGLUNIFORM3FVPROC)glXGetProcAddress((const unsigned char*)"glUniform3fv");
    glUniformMatrix4fv_FA = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((const unsigned char*)"glUniformMatrix4fv");
    glUniformMatrix3fv_FA = (PFNGLUNIFORMMATRIX3FVPROC)glXGetProcAddress((const unsigned char*)"glUniformMatrix3fv");
    glUniformMatrix2fv_FA = (PFNGLUNIFORMMATRIX2FVPROC)glXGetProcAddress((const unsigned char*)"glUniformMatrix2fv");

    //NOTE(Stanisz13): PROGRAMS
    glCreateProgram_FA = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glCreateProgram");
    glLinkProgram_FA = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glLinkProgram");
    glGetProgramiv_FA = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((const unsigned char*)"glGetProgramiv");
    glGetProgramInfoLog_FA = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((const unsigned char*)"glGetProgramInfoLog");
    glDeleteProgram_FA = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glDeleteProgram");
    glUseProgram_FA = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glUseProgram");

    //NOTE(Stanisz13): BUFFERS
    glGenVertexArrays_FA = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glGenVertexArrays");
    glGenBuffers_FA = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glGenBuffers");
    glGenVertexArrays_FA = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glGenVertexArrays");
    glBindBuffer_FA = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const unsigned char*)"glBindBuffer");
    glBufferData_FA = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const unsigned char*)"glBufferData");
    glVertexAttribPointer_FA = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const unsigned char*)"glVertexAttribPointer");
    glEnableVertexAttribArray_FA = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const unsigned char*)"glEnableVertexAttribArray");
    glUseProgram_FA = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glUseProgram");
    glBindVertexArray_FA = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((const unsigned char*)"glBindVertexArray");
    glDeleteBuffers_FA = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glDeleteBuffers");
    glDeleteVertexArrays_FA = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glDeleteVertexArrays");

    //NOTE(Stanisz13): V_SYNC
    glXSwapIntervalMESA_FA = (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress((const unsigned char*)"glXSwapIntervalMESA");
    glXSwapIntervalEXT_FA = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const unsigned char*)"glXSwapIntervalEXT");
    glXSwapIntervalSGI_FA = (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress((const unsigned char*)"glXSwapIntervalSGI");
}

unsigned colorToUnsigned(const Color* c)
{
    unsigned res = 0;

    res |= ((unsigned)c->r << 0);
    res |= ((unsigned)c->g << 8);
    res |= ((unsigned)c->b << 16);
    res |= ((unsigned)c->a << 24);

    return res;
}

Color unsignedToColor(unsigned mask)
{
    Color res;

    res.r = (unsigned char)(mask);
    res.g = (unsigned char)(mask >> 8);
    res.b = (unsigned char)(mask >> 16);
    res.a = (unsigned char)(mask >> 24);

    return res;
}

void createTextureForDrawingBuffer(PixelBufferData* pdata)
{
    glGenTextures(1, &pdata->texture);
    glBindTexture(GL_TEXTURE_2D, pdata->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float vertices[] = {
        // positions       // texture coords
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    unsigned indices[] = {
        0, 1, 3, 2
    };

    unsigned int VBO, EBO;
    glGenVertexArrays_FA(1, &pdata->VAO);
    glGenBuffers_FA(1, &VBO);
    glGenBuffers_FA(1, &EBO);

    glBindVertexArray_FA(pdata->VAO);

    glBindBuffer_FA(GL_ARRAY_BUFFER, VBO);
    glBufferData_FA(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer_FA(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData_FA(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer_FA(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray_FA(0);
    // texture coord attribute
    glVertexAttribPointer_FA(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray_FA(1);
}

void drawTextureWithBufferData(PixelBufferData* pdata)
{
    glBindTexture(GL_TEXTURE_2D, pdata->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, contextData_FA.windowWidth, contextData_FA.windowHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pdata->pixels);

    glUseProgram_FA(pdata->basicProgram);
    glBindVertexArray_FA(pdata->VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
}

void freePixelData(PixelBufferData* pdata)
{
    free(pdata->pixels);
}

float lerp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

Color lerpColor(const Color* a, const Color* b, const float t)
{
    Color res;
    res.r = (unsigned char)lerp(a->r, b->r, t);
    res.g = (unsigned char)lerp(a->g, b->g, t);
    res.b = (unsigned char)lerp(a->b, b->b, t);
    res.a = (unsigned char)lerp(a->a, b->a, t);

    return res;
}

void configurePingpongBuffer(PingpongBuffer* pbuf)
{
    glGenFramebuffers_FA(2, pbuf->fbo);
    glGenTextures(2, pbuf->texture);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer_FA(GL_FRAMEBUFFER, pbuf->fbo[i]);
        glBindTexture(GL_TEXTURE_2D, pbuf->texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
                     contextData_FA.windowWidth, contextData_FA.windowHeight,
                     0, GL_RED, GL_FLOAT, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D_FA(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, pbuf->texture[i], 0);
    }

}

void configureScreenQuadWithEBO(ScreenQuadWithEBO* squad)
{
    float screenQuadVerts[] =
        {
            1.0f,  1.0f,
            -1.0f, 1.0f,
            -1.0f, -1.0f,
            1.0f, -1.0f,
        };

    unsigned indices[] =
        {
            0, 1, 3, 2
        };

    glGenVertexArrays_FA(1, &squad->VAO);
    glGenBuffers_FA(1, &squad->VBO);
    glGenBuffers_FA(1, &squad->EBO);

    glBindVertexArray_FA(squad->VAO);

    glBindBuffer_FA(GL_ARRAY_BUFFER, squad->VBO);
    glBufferData_FA(GL_ARRAY_BUFFER, sizeof(screenQuadVerts), &screenQuadVerts, GL_STATIC_DRAW);

    glBindBuffer_FA(GL_ELEMENT_ARRAY_BUFFER, squad->EBO);
    glBufferData_FA(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer_FA(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray_FA(0);
}

void configureScreenQuad(ScreenQuad* squad)
{
    float screenQuadVerts[] =
        {
            1.0f,  1.0f,
            -1.0f, 1.0f,
            -1.0f, -1.0f,
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f
        };

    glGenVertexArrays_FA(1, &squad->VAO);
    glGenBuffers_FA(1, &squad->VBO);

    glBindVertexArray_FA(squad->VAO);

    glBindBuffer_FA(GL_ARRAY_BUFFER, squad->VBO);
    glBufferData_FA(GL_ARRAY_BUFFER, sizeof(screenQuadVerts), &screenQuadVerts, GL_STATIC_DRAW);

    glVertexAttribPointer_FA(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray_FA(0);
}


void freeScreenQuad(ScreenQuad* squad)
{
    glDeleteBuffers_FA(1, &squad->VBO);
    glDeleteVertexArrays_FA(1, &squad->VAO);
}

void freeScreenQuadWithEBO(ScreenQuadWithEBO* squad)
{
    glDeleteBuffers_FA(1, &squad->VBO);
    glDeleteVertexArrays_FA(1, &squad->VAO);
    glDeleteBuffers_FA(1, &squad->EBO);
}

void loadEntireFile(const char* path, unsigned char** source)
{
    FILE* file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    unsigned fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    *source = (unsigned char *)calloc(1, fileSize + 1);
    memset(*source, 0, fileSize + 1);

    if (fread(*source, fileSize, 1, file) != 1) {
        logError("Failed reading file!");
        logError(path);

        return;
    }

    fclose(file);
}

unsigned createShaderProgram(const char* pathToVS, const char* pathToFS)
{
    unsigned vertex, fragment;
    unsigned program;
    int success;
    char infoLog[512];

    unsigned char* vsCode;
    unsigned char* fsCode;
    loadEntireFile(pathToVS, &vsCode);
    loadEntireFile(pathToFS, &fsCode);

    vertex = glCreateShader_FA(GL_VERTEX_SHADER);
    fragment = glCreateShader_FA(GL_FRAGMENT_SHADER);
    glShaderSource_FA(vertex, 1, (const char**)&vsCode, NULL);
    glShaderSource_FA(fragment, 1, (const char**)&fsCode, NULL);

    glCompileShader_FA(vertex);
    glCompileShader_FA(fragment);

    glGetShaderiv_FA(vertex, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog_FA(vertex, 512, NULL, infoLog);
        printf("vertex shader error: %s\n", infoLog);
    }

    glGetShaderiv_FA(fragment, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog_FA(fragment, 512, NULL, infoLog);
        printf("fragment shader error: %s\n", infoLog);
    }

    program = glCreateProgram_FA();
    glAttachShader_FA(program, vertex);
    glAttachShader_FA(program, fragment);
    glLinkProgram_FA(program);

    glGetProgramiv_FA(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog_FA(program, 512, NULL, infoLog);
        printf("%s\n", infoLog);
    }

    glDeleteShader_FA(vertex);
    glDeleteShader_FA(fragment);

    free(vsCode);
    free(fsCode);

    return program;
}


void disableVSyncIfPossible()
{
    if (vSyncData_FA.mask == 0)
    {
        logWarning("No extension supporting v-sync control found! Will run without it...");
        return;
    }

    logWarning("V-SYNC DISABLED!");

    if (vSyncData_FA.mask & MESA_V_SYNC)
    {
        glXSwapIntervalMESA_FA(0);
        return;
    }

    if (vSyncData_FA.mask & EXT_V_SYNC)
    {
        glXSwapIntervalEXT_FA(contextData_FA.display, glXGetCurrentDrawable(), 0);
        return;
    }

    if (vSyncData_FA.mask & SGI_V_SYNC)
    {
        glXSwapIntervalSGI_FA(0);
        return;
    }
}

void enableVSyncIfPossible()
{
    if (vSyncData_FA.mask == 0)
    {
        logWarning("No extension supporting v-sync control found! Will run without it...");
        return;
    }

    logWarning("V-SYNC ENABLED!");

    if (vSyncData_FA.mask & MESA_V_SYNC)
    {
        glXSwapIntervalMESA_FA(1);
        return;
    }

    if (vSyncData_FA.mask & EXT_V_SYNC)
    {
        glXSwapIntervalEXT_FA(contextData_FA.display, glXGetCurrentDrawable(), 1);
        return;
    }

    if (vSyncData_FA.mask & SGI_V_SYNC)
    {
        glXSwapIntervalSGI_FA(1);
        return;
    }
}


void enableAdaptiveVSyncIfPossible()
{
    if (vSyncData_FA.mask == 0)
    {
        logWarning("No extension supporting v-sync control found! Will run without configuring it...");
        return;
    }

    logWarning("ADAPTIVE V-SYNC ENABLED!");

    if (vSyncData_FA.mask & MESA_V_SYNC)
    {
        glXSwapIntervalMESA_FA(-1);
        return;
    }

    if (vSyncData_FA.mask & EXT_V_SYNC)
    {
        glXSwapIntervalEXT_FA(contextData_FA.display, glXGetCurrentDrawable(), -1);
        return;
    }

    if (vSyncData_FA.mask & SGI_V_SYNC)
    {
        glXSwapIntervalSGI_FA(-1);
        return;
    }
}

#pragma pack(push, 1)
typedef struct 
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t bitmapOffset;
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t sizeOfBitmap;
    int32_t horzResolution;
    int32_t vertResolution;
    uint32_t colorsUsed;
    uint32_t colorsImportant;

} BitmapHeader;
#pragma pack(pop)

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t* pixels;

} BMPImage;

unsigned loadBMPtexture(const char* filename, unsigned* texture)
{
    FILE* inFile = fopen(filename, "rb");

    unsigned success = 1;
    
    if (inFile)
    {
        BitmapHeader header;

        fread(&header, sizeof(header), 1, inFile);
        uint32_t inputPixelsSize = header.width * header.height * sizeof(unsigned);

        BMPImage image;
        image.width = header.width;
        image.height = header.height;
        image.pixels = (unsigned*)malloc(inputPixelsSize);
        
        fread(image.pixels, inputPixelsSize, 1, inFile);

        //NOTE(Stanisz13): Gimp exports 32-bit BMPs in ARGB memory layout.
        // The layout that is expected on linux machine (possibly to
        // match OpenGL expectancy) is ABGR. Thus, there is a need to
        // swap R and B channels.        
        unsigned *running = image.pixels;
        for (unsigned i = 0; i < inputPixelsSize; i += 4)
        {            
            unsigned raw = *running;
            unsigned char red = (raw >> 16);
            unsigned char blue = (raw);

            unsigned newVal = (raw & 0xff00ff00);
            newVal |= (red);
            newVal |= (blue << 16);

            *running = newVal;

            ++running;
        }

        fclose(inFile);

        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);
        glGenerateMipmap_FA(GL_TEXTURE_2D);
        glActiveTexture_FA(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *texture);

        free(image.pixels);
    }
    else
    {
        logError("Unable to read BMP file!");
        success = 0;
    }

    return success;
}

void mouseCoordsToNDC()
{
    mouseState_FA.posX *= 2;
    mouseState_FA.posX -= contextData_FA.windowWidth;

    mouseState_FA.posY *= 2;
    mouseState_FA.posY -= contextData_FA.windowHeight;
    mouseState_FA.posY *= -1.0f;

    mouseState_FA.posX /= contextData_FA.windowWidth;
    mouseState_FA.posY /= contextData_FA.windowHeight;
}
