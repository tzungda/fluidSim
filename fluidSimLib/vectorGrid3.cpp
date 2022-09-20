//---vectorGrid3.cpp

#include "vectorGrid3.h"

vectorGrid3::vectorGrid3()
{
}

vectorGrid3::~vectorGrid3()
{
}

void vectorGrid3::resize(const size3& resolution,
    const vector3& gridSpacing,
    const vector3& origin,
    const vector3& initValue)
{
    setSize(resolution, gridSpacing, origin);

    onResize(resolution, gridSpacing, origin, initValue);
}

