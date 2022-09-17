//--- cellCenteredVectorGrid3.h
#ifndef cellCenteredVectorGrid3_H
#define cellCenteredVectorGrid3_H

#include "extendVectorGrid3.h"

class cellCenteredVectorGrid3 final : public extendVectorGrid3 {
public:

    cellCenteredVectorGrid3();


    cellCenteredVectorGrid3(
        size_t resolutionX,
        size_t resolutionY,
        size_t resolutionZ,
        double gridSpacingX = 1.0,
        double gridSpacingY = 1.0,
        double gridSpacingZ = 1.0,
        double originX = 0.0,
        double originY = 0.0,
        double originZ = 0.0,
        double initialValueU = 0.0,
        double initialValueV = 0.0,
        double initialValueW = 0.0);

    cellCenteredVectorGrid3(
        const size3& resolution,
        const vector3& gridSpacing = vector3(1.0, 1.0, 1.0),
        const vector3& origin = vector3(),
        const vector3& initialValue = vector3());

   
    cellCenteredVectorGrid3(const cellCenteredVectorGrid3& other);

    size3 dataSize() const override;

    vector3 dataOrigin() const override;
   
    std::shared_ptr<vectorGrid3> clone() const override;

    void swap(grid3* other) override;

    void fill(const vector3& value) override;

    void fill(const std::function<vector3(const vector3&)>& func) override;

    void set(const cellCenteredVectorGrid3& other);

    cellCenteredVectorGrid3& operator=(const cellCenteredVectorGrid3& other);

};

//! Shared pointer for the cellCenteredVectorGrid3 type.
typedef std::shared_ptr<cellCenteredVectorGrid3> cellCenteredVectorGrid3Ptr;

#endif


