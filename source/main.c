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

        // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays_FA(1, &VAO);
    glGenBuffers_FA(1, &VBO);
    glGenBuffers_FA(1, &EBO);

    glBindVertexArray_FA(VAO);

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

    float aRatio = (float)contextData.windowWidth / contextData.windowHeight;


    unsigned tex;
    loadBMPtexture("beauty.bmp", &tex);

    int basic = createShaderProgram("shaders/basic.vs", "shaders/basic.fs");
    glUseProgram_FA(basic);

    glEnable(GL_DEPTH_TEST);


    unsigned modelLoc = glGetUniformLocation_FA(basic, "model");
    unsigned projLoc = glGetUniformLocation_FA(basic, "proj");
    unsigned viewLoc = glGetUniformLocation_FA(basic, "view");

    FVec3 scalingV = initFVec3(0.5f, 0.5f, 1.0f);
    FMat4 model = translationFMat4(initFVec3(0.0f, 0.0f, -5.0f));
    model = mulFMat4(model, rotationFMat4(degreesToRadians(-60.0f), initFVec3(1.0f, 1.0f, 0.0f)));
    FMat4 proj = projectionFMat4(0.01f, 10.0f, 1.0f, degreesToRadians(32.0f));
    FMat4 view = identityFMat4();
    
    glUniformMatrix4fv_FA(modelLoc, 1, GL_FALSE, model.mem);
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

        while (XPending(contextData.display))
        {
            XNextEvent(contextData.display, &event);
            switch (event.type)
            {
                case ClientMessage:
                    if (event.xclient.data.l[0] == contextData.deleteMessage)
                        isRunning = 0;
                    break;
            }
        }

        if (isRunning == 0)
        {
            break;
        }

        glClearColor(0, 0.5, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_EBO_TO_DRAW_QUAD == 1
        glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
#else
        glDrawArrays(GL_TRIANGLES, 0, 6);
#endif

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
    glDeleteBuffers_FA(1, &EBO);

    return 0;
}
