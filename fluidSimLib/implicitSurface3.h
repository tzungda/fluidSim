//---implicitSurface3.h
#ifndef implicitSurface3_H
#define implicitSurface3_H

#include "surface3.h"

class implicitSurface3 : public surface3
{
public:
    //! Default constructor.
    implicitSurface3(
        const transform3& transform = transform3(),
        bool isNormalFlipped = false);

    //! Copy constructor.
    implicitSurface3(const implicitSurface3& other);

    //! Default destructor.
    virtual ~implicitSurface3();

    //! Returns signed distance from the given point \p otherPoint.
    double signedDistance(const vector3& otherPoint) const;

protected:
    //! Returns signed distance from the given point \p otherPoint in local
    //! space.
    virtual double signedDistanceLocal(const vector3& otherPoint) const = 0;

private:
    double closestDistanceLocal(const vector3& otherPoint) const override;
};

typedef std::shared_ptr< implicitSurface3 > implicitSurface3Ptr;

#endif

