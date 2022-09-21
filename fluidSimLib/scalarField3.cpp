//---scalarField3.cpp

#include "scalarField3.h"

scalarField3::scalarField3()
{
}

scalarField3::~scalarField3()
{
}

vector3 scalarField3::gradient(const vector3&) const
{
    return vector3();
}

FloatType scalarField3::laplacian(const vector3&) const
{
    return 0.0;
}

std::function<FloatType(const vector3&)> scalarField3::sampler() const
{
    const scalarField3* self = this;
    return [self](const vector3& x) -> FloatType {
        return self->sample(x);
    };
}

constantScalarField3::constantScalarField3(FloatType value): mValue( value )
{
}


FloatType constantScalarField3::sample(const vector3& x) const
{
    return mValue;
}


