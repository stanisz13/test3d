#include "graphics_FA.h"
#include "maths_FA.h"

int isRunning = 1;

int main(int argc, char* argv[])
{
    newLine();
    newLine();

    ContextData contextData;
    contextData.minimalGLXVersionMajor = 1;
    contextData.minimalGLXVersionMinor = 3;
    contextData.minimalGLVersionMajor = 3;
    contextData.minimalGLVersionMinor = 3;
    contextData.windowWidth = 1920;
    contextData.windowHeight = 1080;
    contextData.name = "Faith";

    UserVSyncData userVSyncData;

    configureOpenGL(&contextData, &userVSyncData);
    loadFunctionPointers();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays_FA(1, &VAO);
    glGenBuffers_FA(1, &VBO);

    glBindVertexArray_FA(VAO);

    glBindBuffer_FA(GL_ARRAY_BUFFER, VBO);
    glBufferData_FA(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer_FA(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray_FA(0);
    // texture coord attribute
    glVertexAttribPointer_FA(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray_FA(1);

    float aRatio = (float)contextData.windowWidth / contextData.windowHeight;


    unsigned tex;
    loadBMPtexture("beauty.bmp", &tex);

    int basic = createShaderProgram("shaders/basic.vs", "shaders/basic.fs");
    glUseProgram_FA(basic);

    glEnable(GL_DEPTH_TEST);


    unsigned modelLoc = glGetUniformLocation_FA(basic, "model");
    unsigned projLoc = glGetUniformLocation_FA(basic, "proj");
    unsigned viewLoc = glGetUniformLocation_FA(basic, "view");
                        
    FMat4 model = translationFMat4(initFVec3(0.0f, 0.0f, -3.0f));
    FMat4 proj = projectionFMat4(0.01f, 10.0f, aRatio, degreesToRadians(45.0f));
    FMat4 view = identityFMat4();
    
    glUniformMatrix4fv_FA(projLoc, 1, GL_FALSE, proj.mem);
    glUniformMatrix4fv_FA(viewLoc, 1, GL_FALSE, view.mem);

#if 0
    disableVSyncIfPossible(&contextData, &userVSyncData);
#else
    enableVSyncIfPossible(&contextData, &userVSyncData);
#endif

    struct timespec prevTime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &prevTime);
    float dt = 0.0f;
    float elapsed = 0.0f;
    float maxFrameTimeNoticed = 0.0f;

    while(1)
    {
        XEvent event;
        mouseState_FA.wheel = 0;

        while (XPending(contextData.display))
        {
            XNextEvent(contextData.display, &event);
            
            switch (event.type)
            {
                case ClientMessage:
                    if (event.xclient.data.l[0] == contextData.deleteMessage)
                        isRunning = 0;
                    break;

                case KeymapNotify:
                    XRefreshKeyboardMapping(&event.xmapping);
                    break;
                    
                case ButtonPress:
                    switch(event.xbutton.button)
                    {
                        case Button1:
                            mouseState_FA.left = 1;
                            break;
                        case Button2:
                            mouseState_FA.middle = 1;
                            break;
                        case Button3:
                            mouseState_FA.right = 1;
                            break;
                        case Button4:
                            mouseState_FA.wheel = 1;
                            break;
                        case Button5:
                            mouseState_FA.wheel = -1;
                            break;
                    }
                    break;

                case ButtonRelease:
                    switch(event.xbutton.button)
                    {
                        case Button1:
                            mouseState_FA.left = 0;
                            break;
                        case Button2:
                            mouseState_FA.middle = 0;
                            break;
                        case Button3:
                            mouseState_FA.right = 0;
                            break;
                    }
                    break;

                case MotionNotify:
                    mouseState_FA.posX = event.xmotion.x;
                    mouseState_FA.posY = event.xmotion.y;
                    break;

                case KeyPress:
                    keysPressed_FA[event.xkey.keycode] = 1;
                    break;
                    
                case KeyRelease:
                    keysPressed_FA[event.xkey.keycode] = 0;
                    break;
            }
        }

        if (keysPressed_FA[9] == 1)
        {
            isRunning = 0;
        }
        
        if (isRunning == 0)
        {
            break;
        }
        
        glClearColor(0, 0.5, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = mulFMat4(model, rotationFMat4(dt/1000, initFVec3(1.0f, 1.0f, 0.3f)));
        glUniformMatrix4fv_FA(modelLoc, 1, GL_FALSE, model.mem);
    

        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glXSwapBuffers(contextData.display, contextData.window);

        struct timespec nowTime;
        clock_gettime(CLOCK_MONOTONIC_RAW, &nowTime);
        dt = (nowTime.tv_sec - prevTime.tv_sec) * 1000000
            + (nowTime.tv_nsec - prevTime.tv_nsec) / 1000;
        dt /= 1000.0f;

        prevTime = nowTime;

        elapsed += dt;
        if (dt - maxFrameTimeNoticed > EPSILON)
        {
            maxFrameTimeNoticed = dt;
        }

#if 1
        if (elapsed > 2.0f * 1000)
        {
            printf("Longest frame: %f [%f FPS]\n", maxFrameTimeNoticed, 1000.0f / maxFrameTimeNoticed);
            elapsed = 0.0f;
            maxFrameTimeNoticed = 0.0f;
        }
#else
        printf("dt = %f [%f FPS]\n", dt, 1.0f/dt);
#endif
    }

    freeContextData(&contextData);
    glDeleteBuffers_FA(1, &VBO);
    glDeleteVertexArrays_FA(1, &VAO);

    return 0;
}
