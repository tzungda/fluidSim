//--- cellCenteredScalarGrid3.h
#ifndef cellCenteredScalarGrid3_H
#define cellCenteredScalarGrid3_H

#include "scalarGrid3.h"

class cellCenteredScalarGrid3 : public scalarGrid3
{
public:
    cellCenteredScalarGrid3();

    cellCenteredScalarGrid3(const size3& resolution, const vector3& gridSpacing = vector3(1.0, 1.0, 1.0),
        const vector3& origin = vector3(), FloatType initValue = 0.0);
    cellCenteredScalarGrid3(const cellCenteredScalarGrid3& other);

    virtual ~cellCenteredScalarGrid3();

    void swap(grid3* other) override;
    void set(const cellCenteredScalarGrid3& other);


    size3 dataSize() const override;
    vector3 dataOrigin() const override;
    std::shared_ptr<scalarGrid3> clone() const override;

};

typedef std::shared_ptr< cellCenteredScalarGrid3 > cellCenteredScalarGrid3Ptr;

#endif
