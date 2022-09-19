//---faceCenteredGrid3.h
#ifndef faceCenteredGrid3_H
#define faceCenteredGrid3_H

#include <memory>
#include <functional>

#include "vectorGrid3.h"
#include "dataBuffer3.h"
#include "dataBufferSamplers.h"

class faceCenteredGrid3 : public vectorGrid3
{
private:
    dataBuffer3 mDataU;
    dataBuffer3 mDataV;
    dataBuffer3 mDataW;

    vector3 mDataOriginU;
    vector3 mDataOriginV;
    vector3 mDataOriginW;

    LinearBufferSampler mLinearSamplerU;
    LinearBufferSampler mLinearSamplerV;
    LinearBufferSampler mLinearSamplerW;
    std::function<vector3(const vector3&)> mSampler;

    void resetSampler();

public:
    faceCenteredGrid3();
    faceCenteredGrid3(
        const size3& resolution,
        const vector3& gridSpacing = vector3(1.0, 1.0, 1.0),
        const vector3& origin = vector3(),
        const vector3& initialValue = vector3());
    faceCenteredGrid3(const faceCenteredGrid3& other);
    virtual ~faceCenteredGrid3();

    void set(const faceCenteredGrid3& other);

    FloatType &u(size_t i, size_t j, size_t k);
    FloatType &v(size_t i, size_t j, size_t k);
    FloatType &w(size_t i, size_t j, size_t k);

    const FloatType &u(size_t i, size_t j, size_t k) const;
    const FloatType &v(size_t i, size_t j, size_t k) const;
    const FloatType &w(size_t i, size_t j, size_t k) const;

    FloatType &u( const size3& pt );
    FloatType &v( const size3& pt );
    FloatType &w( const size3& pt );

    const FloatType &u( const size3& pt ) const;
    const FloatType &v( const size3& pt ) const;
    const FloatType &w( const size3& pt ) const;

    vector3 uPosition(size_t i, size_t j, size_t k) const;
    vector3 vPosition(size_t i, size_t j, size_t k) const;
    vector3 wPosition(size_t i, size_t j, size_t k) const;


    DataPositionFunc uPosition() const;
    DataPositionFunc vPosition() const;
    DataPositionFunc wPosition() const;
    size3 uSize() const;
    size3 vSize() const;
    size3 wSize() const;

    dataBuffer3& uData();
    const dataBuffer3& uData() const;
    dataBuffer3& vData();
    const dataBuffer3& vData() const;
    dataBuffer3& wData();
    const dataBuffer3& wData() const;

    vector3 uOrigin() const { return mDataOriginU; }
    vector3 vOrigin() const { return mDataOriginV; }
    vector3 wOrigin() const { return mDataOriginW; }

    void forEachUIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;
    void forEachVIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;
    void forEachWIndex(
        const std::function<void(size_t, size_t, size_t)>& func) const;

    FloatType divergenceAtCellCenterByIndex(size_t i, size_t j, size_t k) const;

    vector3 valueAtCellCenterByIndex(size_t i, size_t j, size_t k) const;
    vector3 curlAtCellCenterByIndex(size_t i, size_t j, size_t k) const;

    std::shared_ptr<vectorGrid3> clone() const override;
    vector3 sample(const vector3& x) const override;
    void fill(const vector3& value) override;
    void fill(const std::function<vector3(const vector3&)>& func) override;
    void swap(grid3* other) override;

protected:
    void onResize(const size3& resolution, const vector3& gridSpacing, const vector3& origin, const vector3& initValue) override;
    void getData(std::vector<FloatType>* data) const override;
    void setData(const std::vector<FloatType>& data) override;
};

typedef std::shared_ptr<faceCenteredGrid3> faceCenteredGrid3Ptr;

#endif
