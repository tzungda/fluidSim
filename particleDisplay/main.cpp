//---main.cpp

#include "sceneparticles.h"

#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <stdio.h>
#include <iostream>


#define WIN_WIDTH 800
#define WIN_HEIGHT 600


GLFWwindow * window = nullptr;
int fbw, fbh;

void dumpGLInfo(bool dumpExtensions = false)
{
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor, samples, sampleBuffers;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    glGetIntegerv(GL_SAMPLES, &samples);
    glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);

    printf("-------------------------------------------------------------\n");
    printf("GL Vendor    : %s\n", vendor);
    printf("GL Renderer  : %s\n", renderer);
    printf("GL Version   : %s\n", version);
    printf("GL Version   : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);
    printf("MSAA samples : %d\n", samples);
    printf("MSAA buffers : %d\n", sampleBuffers);
    printf("-------------------------------------------------------------\n");

    if (dumpExtensions) {
        GLint nExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
        for (int i = 0; i < nExtensions; i++) {
            printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
        }
    }
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar * msg, const void * param)
{
    std::string sourceStr;
    switch (source) {
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        sourceStr = "WindowSys";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        sourceStr = "App";
        break;
    case GL_DEBUG_SOURCE_API:
        sourceStr = "OpenGL";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        sourceStr = "ShaderCompiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sourceStr = "3rdParty";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        sourceStr = "Other";
        break;
    default:
        sourceStr = "Unknown";
    }

    std::string typeStr;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        typeStr = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeStr = "Deprecated";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeStr = "Undefined";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeStr = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeStr = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        typeStr = "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        typeStr = "PushGrp";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        typeStr = "PopGrp";
        break;
    case GL_DEBUG_TYPE_OTHER:
        typeStr = "Other";
        break;
    default:
        typeStr = "Unknown";
    }

    std::string sevStr;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        sevStr = "HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        sevStr = "MED";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        sevStr = "LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        sevStr = "NOTIFY";
        break;
    default:
        sevStr = "UNK";
    }

    std::cerr << sourceStr << ":" << typeStr << "[" << sevStr << "]" << "(" << id << "): " << msg << std::endl;
}

void initWindow(const std::string &windowTitle)
{
    // Initialize GLFW
    if (!glfwInit()) exit(EXIT_FAILURE);

#ifdef __APPLE__
    // Select OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    // Select OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //if(debug)
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }
    /*if(samples > 0)
    {
    glfwWindowHint(GLFW_SAMPLES, samples);
    }*/

    // Open the window
    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, windowTitle.c_str(), NULL, NULL);
    if (!window) {
        std::cerr << "Unable to create OpenGL context." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Get framebuffer size
    glfwGetFramebufferSize(window, &fbw, &fbh);

    // Load the OpenGL functions.
    if (!gladLoadGL()) { exit(-1); }

    dumpGLInfo();

    // Initialization
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
#ifndef __APPLE__
    //if (debug)
    {
        glDebugMessageCallback(debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
            GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
    }
#endif
}

int checkForOpenGLError(const char * file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        const char * message = "";
        switch (glErr)
        {
        case GL_INVALID_ENUM:
            message = "Invalid enum";
            break;
        case GL_INVALID_VALUE:
            message = "Invalid value";
            break;
        case GL_INVALID_OPERATION:
            message = "Invalid operation";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            message = "Invalid framebuffer operation";
            break;
        case GL_OUT_OF_MEMORY:
            message = "Out of memory";
            break;
        default:
            message = "Unknown error";
        }

        printf("glError in file %s @ line %d: %s\n", file, line, message);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void mainLoop(GLFWwindow * window, std::unique_ptr<sceneParticles> scene)
{
    scene->setDimensions(fbw, fbh);
    scene->initScene();
    scene->resize(fbw, fbh);

    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        
        checkForOpenGLError(__FILE__, __LINE__);

        scene->update(float(glfwGetTime()));
        scene->render();
        glfwSwapBuffers(window);

        glfwPollEvents();
        int state = glfwGetKey(window, GLFW_KEY_SPACE);
        if (state == GLFW_PRESS)
        {
            if ( scene->animating() )
                printf( "pause anim \n" );
            else
                printf( "activate anim \n" );
            scene->animate(!scene->animating());
        }
        
    }
}

int main(int argc, char *argv[])
{
    initWindow("ParticleDisplay");

    std::unique_ptr<sceneParticles> scene = std::unique_ptr<sceneParticles>(new sceneParticles());

    mainLoop(window, std::move(scene));

    return 1;
}

