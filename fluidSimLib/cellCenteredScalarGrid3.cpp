//--- cellCenteredScalarGrid3.cpp

#include "cellCenteredScalarGrid3.h"

cellCenteredScalarGrid3::cellCenteredScalarGrid3()
{
}

cellCenteredScalarGrid3::cellCenteredScalarGrid3(const size3& resolution, const vector3& gridSpacing, const vector3& origin, double initValue)
{
    resize(resolution, gridSpacing, origin, initValue);
}

cellCenteredScalarGrid3::cellCenteredScalarGrid3(const cellCenteredScalarGrid3& other)
{
    set(other);
}

cellCenteredScalarGrid3::~cellCenteredScalarGrid3()
{
}

void cellCenteredScalarGrid3::swap(grid3* other)
{
    cellCenteredScalarGrid3* sameType
        = dynamic_cast<cellCenteredScalarGrid3*>(other);
    if (sameType != nullptr)
    {
        swapScalarGrid(sameType);
    }
}

void cellCenteredScalarGrid3::set(const cellCenteredScalarGrid3& other)
{
    setScalarGrid(other);
}


size3 cellCenteredScalarGrid3::dataSize() const
{
    return resolution();
}

vector3 cellCenteredScalarGrid3::dataOrigin() const
{
    return origin() + 0.5 * gridSpacing();
}

std::shared_ptr<scalarGrid3> cellCenteredScalarGrid3::clone() const
{
    return std::shared_ptr<cellCenteredScalarGrid3>(new cellCenteredScalarGrid3(*this),
        [](cellCenteredScalarGrid3* obj) 
    {
        delete obj;
    });
}

