//---vertexCenteredScalarGrid3.h
#ifndef vertexCenteredScalarGrid3_H
#define vertexCenteredScalarGrid3_H

#include "scalarGrid3.h"

class vertexCenteredScalarGrid3 : public scalarGrid3
{
public:
    vertexCenteredScalarGrid3();

    vertexCenteredScalarGrid3(const size3& resolution, const vector3& gridSpacing = vector3(1.0, 1.0, 1.0),
        const vector3& origin = vector3(), FloatType initValue = 0.0);

    virtual ~vertexCenteredScalarGrid3();


    size3 dataSize() const override;

    vector3 dataOrigin() const override;
};

#endif

