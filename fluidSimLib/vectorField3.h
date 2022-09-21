//---vectorField3.h
#ifndef vectorField3_H
#define vectorField3_H

#include <memory>
#include <functional>

#include "field3.h"
#include "vector3.h"

class vectorField3 : public field3
{
public:
    vectorField3();

    virtual ~vectorField3();

    virtual vector3 sample(const vector3 &x) const = 0;


    virtual FloatType divergence(const vector3& x) const;


    virtual vector3 curl(const vector3& x) const;

    virtual std::function<vector3(const vector3&)> sampler() const;
};

typedef std::shared_ptr<vectorField3> vectorField3Ptr;


class constantVectorField3 final : public vectorField3 {
public:

    constantVectorField3(const vector3& value);

    vector3 sample(const vector3& x) const override;

    std::function<vector3(const vector3&)> sampler() const override;

private:
    vector3 mValue;
};

typedef std::shared_ptr<constantVectorField3> constantVectorField3Ptr;

#endif

