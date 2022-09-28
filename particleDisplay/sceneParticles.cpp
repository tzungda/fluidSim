//---sceneparticles.cpp
#include "sceneParticles.h"
#include "texture.h"
//#include "particleutils.h"

//
#include <memory>
#include <sstream>
#include <string>
#include <fstream>


#include <random>
#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

#define FRAME_RANGE 100



sceneParticles::sceneParticles()
{
    mFrame = 0;
    //
    mFluidSim.setup(FRAME_RANGE, 50);
    mFluidSim.update(mFrame);
    //
    nParticles = (GLuint)mFluidSim.positions().size();
    //
    angle = 3.8408f;
    isAnimate = true;
}

sceneParticles::~sceneParticles()
{

}

void sceneParticles::initScene()
{
    compileAndLinkShader();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    initBuffers();

    // The particle texture
    glActiveTexture(GL_TEXTURE0);
    Texture::loadTexture("textures/bluewater.png");

    prog.use();
    prog.setUniform("ParticleTex", 0);
    prog.setUniform("ParticleSize", 0.025f);

    flatProg.use();
    flatProg.setUniform("Color", glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
}


void sceneParticles::initBuffers()
{
    // Generate the buffers for particle pos and vel len
    glGenBuffers(1, &particlePos);   
    glGenBuffers(1, &particleVelLen); 

    // Allocate space for all buffers
    int size = nParticles * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, particlePos);
    glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, particleVelLen);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

    // fill particle positions buffer
    posData.resize(nParticles * 3);
    GLfloat *ptr = &posData[0];
    const vector3 *fluidDataPtr = &mFluidSim.positions()[0];
    for (uint32_t i = 0; i < nParticles; i++)
    {
        *ptr++ = (float)(*fluidDataPtr).x;
        *ptr++ = (float)(*fluidDataPtr).y;
        *ptr++ = (float)(*fluidDataPtr).z;
        fluidDataPtr++;
    }
    glBindBuffer(GL_ARRAY_BUFFER, particlePos);
    glBufferSubData(GL_ARRAY_BUFFER, 0, posData.size() * sizeof(float), posData.data());

    // fill vel len buffer
    velLen.resize(nParticles);
    ptr = &velLen[0];
    fluidDataPtr = &mFluidSim.velocities()[0];
    for (unsigned int i = 0; i < nParticles; i++) 
    {
        *ptr++ = (float)(*fluidDataPtr).length()* 0.5;
        fluidDataPtr++;
    }
    glBindBuffer(GL_ARRAY_BUFFER, particleVelLen);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), velLen.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);
    glBindBuffer(GL_ARRAY_BUFFER, particlePos);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, particleVelLen);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);

}


void sceneParticles::update(float t)
{
    if ( !isAnimate )
        return;
    mFrame++;
    mFluidSim.update(mFrame);
}

void sceneParticles::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = glm::lookAt(vec3(3.0f * cos(angle), 1.5f, 3.0f * sin(angle)), vec3(0.0f, 1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    model = mat4(1.0f);

    flatProg.use();
    setMatrices(flatProg);
    grid.render();

    glDepthMask(GL_FALSE);
    prog.use();
    setMatrices(prog);
    //
    GLfloat *ptr = &posData[0];
    const vector3 *fluidDataPtr = &mFluidSim.positions()[0];
    for (uint32_t i = 0; i < nParticles; i++)
    {
        *ptr++ = (float)(*fluidDataPtr).x;
        *ptr++ = (float)(*fluidDataPtr).y;
        *ptr++ = (float)(*fluidDataPtr).z;
        fluidDataPtr++;
    }
    //
    ptr = &velLen[0];
    fluidDataPtr = &mFluidSim.velocities()[0];
    for (unsigned int i = 0; i < nParticles; i++) 
    {
        *ptr++ = (float)(*fluidDataPtr).length() * 0.5;
        fluidDataPtr++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, particlePos);
    glBufferSubData(GL_ARRAY_BUFFER, 0,posData.size() * sizeof(float), posData.data());
    //
    glBindBuffer(GL_ARRAY_BUFFER, particleVelLen);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), velLen.data());
    //
    glBindVertexArray(particles);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
}

void sceneParticles::setMatrices(GLSLProgram & p)
{
    mat4 mv = view * model;
    p.setUniform("MV", mv);
    p.setUniform("Proj", projection);
}

void sceneParticles::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

void sceneParticles::compileAndLinkShader()
{
    try {
        prog.compileShader("shaders/particles_frag.glsl");
        prog.compileShader("shaders/particles_vert.glsl");

        prog.link();
        prog.use();

        flatProg.compileShader("shaders/flat_frag.glsl");
        flatProg.compileShader("shaders/flat_vert.glsl");

        flatProg.link();
    }
    catch (GLSLProgramException &e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

int sceneParticles::currentFrame( ) const
{
    return mFrame;
}
