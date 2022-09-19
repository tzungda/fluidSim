//--- cellCenteredVectorGrid3.cpp

#include "cellCenteredVectorGrid3.h"

cellCenteredVectorGrid3::cellCenteredVectorGrid3()
{
}

cellCenteredVectorGrid3::cellCenteredVectorGrid3(
    SizeType resolutionX,
    SizeType resolutionY,
    SizeType resolutionZ,
    FloatType gridSpacingX,
    FloatType gridSpacingY,
    FloatType gridSpacingZ,
    FloatType originX,
    FloatType originY,
    FloatType originZ,
    FloatType initialValueU,
    FloatType initialValueV,
    FloatType initialValueW)
{
    resize(
        size3( resolutionX, resolutionY, resolutionZ ),
        vector3( gridSpacingX, gridSpacingY, gridSpacingZ),
        vector3( originX, originY, originZ ),
        vector3( initialValueU, initialValueV, initialValueW ));
}

cellCenteredVectorGrid3::cellCenteredVectorGrid3(
    const size3& resolution,
    const vector3& gridSpacing,
    const vector3& origin,
    const vector3& initialValue) 
{
    resize(resolution, gridSpacing, origin, initialValue);
}

cellCenteredVectorGrid3::cellCenteredVectorGrid3(
    const cellCenteredVectorGrid3& other) 
{
    set(other);
}

size3 cellCenteredVectorGrid3::dataSize() const 
{
    return resolution();
}

vector3 cellCenteredVectorGrid3::dataOrigin() const
{
    return origin() + 0.5 * gridSpacing();
}

void cellCenteredVectorGrid3::swap(grid3* other) 
{
    cellCenteredVectorGrid3* sameType
        = dynamic_cast<cellCenteredVectorGrid3*>(other);
    if (sameType != nullptr) 
    {
        swapExtendVectorGrid(sameType);
    }
}

void cellCenteredVectorGrid3::set(const cellCenteredVectorGrid3& other) 
{
    setExtendVectorGrid(other);
}

cellCenteredVectorGrid3& cellCenteredVectorGrid3::operator=(
    const cellCenteredVectorGrid3& other)
{
    set(other);
    return *this;
}

void cellCenteredVectorGrid3::fill(const vector3& value) 
{
    size3 size = dataSize();
    for ( SizeType i = 0; i < size.x; ++i )
    {
        for ( SizeType j = 0; j < size.y; ++j )
        {
            for ( SizeType k = 0; k < size.z; ++k )
            {
                (*this)( i, j , k ) = value;
            }
        }
    }

}

void cellCenteredVectorGrid3::fill(const std::function<vector3(
    const vector3&)>& func) 
{
    size3 size = dataSize();
    DataPositionFunc pos = dataPosition();
    for ( SizeType i = 0; i < size.x; ++i )
    {
        for ( SizeType j = 0; j < size.y; ++j )
        {
            for ( SizeType k = 0; k < size.z; ++k )
            {
                (*this)( i, j , k ) = func(pos(i, j, k));
            }
        }
    }

}

std::shared_ptr<vectorGrid3> cellCenteredVectorGrid3::clone() const
{
    return std::shared_ptr<cellCenteredVectorGrid3>(
        new cellCenteredVectorGrid3(*this),
        [] (cellCenteredVectorGrid3* obj) {
        delete obj;
        });
    
}

