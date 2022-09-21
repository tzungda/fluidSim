//---grid3.h
#ifndef grid3_H
#define grid3_H

#include <functional>
#include <vector>

#include "vector3.h"
#include "size3.h"
#include "boundingBox3.h"

class grid3
{
private:
    size3 mResolution;
    vector3 mGridSpacing;
    vector3 mOrigin;
    boundingBox3 mBoundingBox;
public:
    typedef std::function<vector3( size_t, size_t, size_t )> DataPositionFunc;

    grid3();

    virtual ~grid3();

    const size3& resolution( ) const;

    const vector3& origin( ) const;

    const vector3& gridSpacing( ) const;

    const boundingBox3& boundingBox( ) const;

    void setSize( const size3& resolution, const vector3& gridSpacing, const vector3& origin );

    void setGrid(const grid3& other);

    virtual void swap(grid3* other) = 0;

    vector3 cellCenterPosition( size_t i, size_t j, size_t k ) const;
    DataPositionFunc cellCenterPosition() const;

    void forEachCellIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;

protected:
    void swapGrid(grid3* other);
    //void setGrid(const grid3& other);
    virtual void getData(std::vector<FloatType>* data) const = 0;
    virtual void setData(const std::vector<FloatType>& data) = 0;
};

#endif


