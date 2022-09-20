//---flipSolver3.h
#ifndef flipSolver3_H
#define flipSolver3_H

#include "picSolver3.h"

class flipSolver3 : public picSolver3
{
public:
    flipSolver3();

    flipSolver3(
        const size3& resolution,
        const vector3& gridSpacing,
        const vector3& gridOrigin);

    virtual ~flipSolver3();

protected:

    void transferFromParticlesToGrids() override;


    void transferFromGridsToParticles() override;

private:
    faceCenteredGrid3 mDelta;
};

typedef std::shared_ptr<flipSolver3> flipSolver3Ptr;

#endif


