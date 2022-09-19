//---scalarGrid3.h
#ifndef scalarGrid3_H
#define scalarGrid3_H

#include <memory>

#include "grid3.h"
#include "scalarField3.h"
#include "dataBuffer3.h"
#include "dataBufferSamplers.h"

class scalarGrid3 : public scalarField3, public grid3
{
private:
    dataBuffer3 mData;
    LinearBufferSampler mLinearSampler;
    std::function<FloatType(const vector3&) > mSampler;

public:
    scalarGrid3();
    virtual ~scalarGrid3();

    virtual size3 dataSize() const = 0;

    virtual vector3 dataOrigin() const = 0;

    virtual std::shared_ptr<scalarGrid3> clone() const = 0;

    void resize(const size3& resolution, const vector3 &gridSpacing = vector3(1.0, 1.0, 1.0),
        const vector3& origin = vector3(), FloatType initValue = 0.0);

    void setScalarGrid(const scalarGrid3& other);

    DataPositionFunc dataPosition() const;

    //const FloatType& operator()( size_t i, size_t j, size_t k ) const;
    vector3 gradientByIndex(size_t i, size_t j, size_t k) const;

    vector3 gradientAtPoint(const vector3& x) const;

    FloatType laplacianByIndex(size_t i, size_t j, size_t k) const;

    void forEachDataPointIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;

    //size3 size( ) const;
    dataBuffer3& data();
    const dataBuffer3& data() const;
    FloatType& operator()(size_t i, size_t j, size_t k);
    const FloatType& operator()(size_t i, size_t j, size_t k) const;

    FloatType sample(const vector3 &x) const override;

    std::function<FloatType(const vector3&)> sampler() const override;

    void fill(FloatType value);
    void fill(const std::function<FloatType(const vector3&)>& func);

protected:
    void swapScalarGrid(scalarGrid3* other);

    //void setScalarGrid(const scalarGrid3& other);

    void getData(std::vector<FloatType>* data) const override;

    void setData(const std::vector<FloatType>& data) override;

private:
    void resetSampler();
};

typedef std::shared_ptr<scalarGrid3> scalarGrid3Ptr;

#endif