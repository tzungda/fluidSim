//---pointHashGridSearcher3.h
#ifndef pointHashGridSearcher3_H
#define pointHashGridSearcher3_H

#include "pointNeighborSearcher3.h"
#include "size3.h"

class pointHashGridSearcher3 : public pointNeighborSearcher3
{
public:


    std::string typeName() const override {return "pointHashGridSearcher3";}

    pointHashGridSearcher3(const size3& resolution, double gridSpacing);

    pointHashGridSearcher3(
        size_t resolutionX,
        size_t resolutionY,
        size_t resolutionZ,
        double gridSpacing);

    pointHashGridSearcher3(const pointHashGridSearcher3& other);

    void build(const std::vector<vector3>& points) override;

    void forEachNearbyPoint(
        const vector3& origin,
        double radius,
        const ForEachNearbyPointFunc& callback) const override;

    bool hasNearbyPoint(
        const vector3& origin, double radius) const override;

    void add(const vector3& point);

    const std::vector<std::vector<size_t>>& buckets() const;

    size_t getHashKeyFromBucketIndex(const size3& bucketIndex) const;

    size3 getBucketIndex(const vector3& position) const;

    pointNeighborSearcher3Ptr clone() const override;

    pointHashGridSearcher3& operator=(const pointHashGridSearcher3& other);

    void set(const pointHashGridSearcher3& other);


private:
    double mGridSpacing = 1.0;
    size3 mResolution = size3(1, 1, 1);
    std::vector<vector3> mPoints;
    std::vector<std::vector<size_t>> mBuckets;

    size_t getHashKeyFromPosition(const vector3& position) const;

    void getNearbyKeys(const vector3& position, size_t* bucketIndices) const;
};

typedef std::shared_ptr< pointHashGridSearcher3 > pointHashGridSearcher3Ptr;

#endif