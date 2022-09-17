//---vectorGrid3.h
#ifndef vectorGrid3_H
#define vectorGrid3_H

#include <memory>

#include "vectorField3.h"
#include "grid3.h"

class vectorGrid3 : public vectorField3, public grid3
{
public:
    vectorGrid3();
    virtual ~vectorGrid3();

    void resize(const size3& resolution,
        const vector3& gridSpacing = vector3(1.0, 1.0, 1.0),
        const vector3& origin = vector3(),
        const vector3& initValue = vector3());

    virtual std::shared_ptr<vectorGrid3> clone() const = 0;

    virtual void fill(const vector3& value) = 0;

    virtual void fill(const std::function<vector3(const vector3&)>& func) = 0;

protected:
    virtual void onResize(const size3& resolution, const vector3& gridSpacing, const vector3& origin, const vector3& initValue) = 0;
};

typedef std::shared_ptr<vectorGrid3> vectorGrid3Ptr;

#endif

