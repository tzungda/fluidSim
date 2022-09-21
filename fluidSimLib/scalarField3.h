//---scalarField3.h
#ifndef scalarField3_H
#define scalarField3_H

#include <memory>
#include <functional>

#include "field3.h"
#include "vector3.h"


class scalarField3 : public field3
{
public:
    scalarField3();

    virtual ~scalarField3();

    virtual FloatType sample(const vector3 &x) const = 0;

    virtual vector3 gradient(const vector3& x) const;

    virtual FloatType laplacian(const vector3& x) const;

    virtual std::function<FloatType(const vector3&)> sampler() const;
};

typedef std::shared_ptr<scalarField3> scalarField3Ptr;


class constantScalarField3 : public scalarField3
{
public:
    constantScalarField3(FloatType value);

    FloatType sample(const vector3& x) const override;
private:
    FloatType mValue;
};

typedef std::shared_ptr<constantScalarField3> constantScalarField3Ptr;

#endif

