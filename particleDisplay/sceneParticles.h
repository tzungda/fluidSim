//---sceneparticles.h
#ifndef sceneParticles_H
#define sceneParticles_H

#include "grid.h"
#include "glslprogram.h"

#include <glad.h>

#include <glm/glm.hpp>
#include <random>


//
#include <flipSolver3.h>
#include <volumeGridEmitter3.h>
#include <dataBuffer2.h>
#include <surfaceToImplicit3.h>
#include <plane3.h>
#include <sphere3.h>
#include <volumeParticleEmitter3.h>
#include <particleEmitterSet3.h>
#include <gridPointGenerator3.h>
#include <frame.h>
//

class fluidSim
{
private:
    int mCurrentFrame;
    int mFrameLength;
    flipSolver3Ptr mSolver;
    double mFPS;
    size_t mResolution;
public:
    fluidSim( ):
        mCurrentFrame( 0 ), mFrameLength( 100 ), mFPS( 60.0 ), mResolution( 50 )
    {

    }
    virtual ~fluidSim()
    {
    }

    void setup( int frameLength, int resolution )
    {
        mFrameLength = frameLength;
        mResolution = resolution;

        // create solver
        mSolver = std::shared_ptr<flipSolver3>(
            new flipSolver3(
                size3( mResolution, 2 * mResolution, mResolution),
                vector3( 1.0/(double)mResolution, 1.0/(double)mResolution, 1.0/(double)mResolution ),
                vector3( 0.0, 0.0, 0.0 )),
            [] (flipSolver3* obj) {
                delete obj;
            });
        //
        const gridSystemData3Ptr& grids = mSolver->gridSystemData();


        vector3 gridSpacing = grids->gridSpacing();
        double dx = gridSpacing.x;
        boundingBox3 domain = grids->boundingBox();

        // emitter
        sphere3Ptr sphere = std::shared_ptr< sphere3 >(
            new sphere3( domain.midPoint(), domain.width() * 0.25 ),
            [] ( sphere3* obj ){
                delete obj;
            });

        implicitSurface3Ptr implicitSphere = std::make_shared<surfaceToImplicit3>(sphere);
        volumeParticleEmitter3Ptr emitter2 = std::shared_ptr< volumeParticleEmitter3 >(
            new volumeParticleEmitter3( implicitSphere, domain, 0.5 * dx ),
            [] (volumeParticleEmitter3* obj ){
                delete obj;
            });
        emitter2->setPointGenerator(std::make_shared<gridPointGenerator3>());
        //
        particleEmitterSet3Ptr emitterSet = std::shared_ptr<particleEmitterSet3>(
            new particleEmitterSet3( {emitter2} ),
            []( particleEmitterSet3 *obj ){
                delete obj;
            });


        mSolver->setParticleEmitter( emitterSet );
    }

    void update( int frameNumber )
    {
        if ( frameNumber > mFrameLength )
        {
            return;
        }

        printf( "fluid sim updating frame: %d\n", frameNumber );

        frame f(0, 1.0 / mFPS);
        f.index = (size_t)frameNumber;
        mSolver->update( f );
    }

    const std::vector<vector3>& positions( )
    {
        const particleSystemData3Ptr& particles = mSolver->particleSystemData();
        return particles->positions( );
    }

    const std::vector<vector3>& velocities( )
    {
        const particleSystemData3Ptr& particles = mSolver->particleSystemData();
        return particles->velocities( );
    }
};

class sceneParticles
{
public:
    int width;
    int height;
    bool isAnimate;
    void setDimensions( int w, int h )
    {
        width = w;
        height = h;
    }

    void animate( bool value ) { isAnimate = value; }
    bool animating() { return isAnimate; }
protected:
    glm::mat4 model, view, projection;

private:

    GLSLProgram prog, flatProg;
    GLuint particlePos, particleVelLen, particles, nParticles;

    Grid grid;

    float angle;

    //
    fluidSim mFluidSim;
    //

    int mFrame;

    void setMatrices(GLSLProgram &);
    void compileAndLinkShader();
    void initBuffers();

    std::vector<GLfloat> posData;
    std::vector<GLfloat> velLen;

public:
    sceneParticles();

    ~sceneParticles( );

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    int  currentFrame( ) const;
};

#endif 