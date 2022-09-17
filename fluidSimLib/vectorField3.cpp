//---vectorField3.cpp

#include "vectorField3.h"

vectorField3::vectorField3() 
{
}

vectorField3::~vectorField3() 
{
}

double vectorField3::divergence(const vector3&) const
{
    return 0.0;
}

vector3 vectorField3::curl(const vector3&) const
{
    return vector3();
}

std::function<vector3(const vector3&)> vectorField3::sampler() const
{
    const vectorField3* self = this;
    return [self](const vector3& x) -> vector3 {
        return self->sample(x);
    };
}


constantVectorField3::constantVectorField3(const vector3& value) :
    mValue(value)
{
}

vector3 constantVectorField3::sample(const vector3& x) const
{
    return mValue;
}

std::function<vector3(const vector3&)> constantVectorField3::sampler() const
{
    return [this](const vector3&) -> vector3 {
        return mValue;
    };
}


