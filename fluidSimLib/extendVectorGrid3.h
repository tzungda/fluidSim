//---extendVectorGrid3.h
#ifndef extendVectorGrid3_H
#define extendVectorGrid3_H

#include "vectorGrid3.h"
#include "vecDataBuffer3.h"
#include "dataBufferSamplers.h"

class extendVectorGrid3 : public vectorGrid3 // the CollocatedVectorGrid3 class of the book
{
private:
    vecDataBuffer3 mData;
    LinearVecBufferSampler mLinearSampler;
    std::function<vector3(const vector3&) > mSampler;

public:
    extendVectorGrid3();
    virtual ~extendVectorGrid3();

    virtual size3 dataSize() const = 0;

    virtual vector3 dataOrigin() const = 0;

    FloatType divergenceByIndex(SizeType i, SizeType j, SizeType k) const;

    vector3 curlByIndex(SizeType i, SizeType j, SizeType k) const;

    void forEachDataPointIndex(
        const std::function<void(SizeType, SizeType, SizeType)>& func) const;

    DataPositionFunc dataPosition() const;

    //const vector3& operator() ( SizeType i, SizeType j, SizeType k ) const;
    vecDataBuffer3& data();
    const vecDataBuffer3& data() const;
    vector3& operator() (SizeType i, SizeType j, SizeType k);
    const vector3& operator() (SizeType i, SizeType j, SizeType k) const;

    // void valueByIndex( SizeType i, SizeType j, SizeType k )

    vector3 sample(const vector3& x) const override;

protected:
    void swapExtendVectorGrid(extendVectorGrid3* other);
    void setExtendVectorGrid(const extendVectorGrid3& other);

    void getData(std::vector<FloatType>* data) const override;
    void setData(const std::vector<FloatType>& data) override;

private:
    void onResize(
        const size3& resolution,
        const vector3& gridSpacing,
        const vector3& origin,
        const vector3& initialValue) final;

    void resetSampler();
};

typedef std::shared_ptr< extendVectorGrid3 > extendVectorGrid3Ptr;

#endif