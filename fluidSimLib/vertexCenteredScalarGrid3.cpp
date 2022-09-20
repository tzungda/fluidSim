//---vertexCenteredScalarGrid3.cpp

#include "vertexCenteredScalarGrid3.h"

vertexCenteredScalarGrid3::vertexCenteredScalarGrid3()
{
}

vertexCenteredScalarGrid3::vertexCenteredScalarGrid3(const size3& resolution, const vector3& gridSpacing, const vector3& origin, double initValue)
{
    resize(resolution, gridSpacing, origin, initValue);
}

vertexCenteredScalarGrid3::~vertexCenteredScalarGrid3()
{
}


size3 vertexCenteredScalarGrid3::dataSize() const
{
    return resolution() + size3(1, 1, 1);
}

vector3 vertexCenteredScalarGrid3::dataOrigin() const
{
    return origin();
}

