//---pointHashGridSearcher3.cpp

#include <basetsd.h>
#include <algorithm>
#include <cmath>

#include "pointHashGridSearcher3.h"

pointHashGridSearcher3::pointHashGridSearcher3(
    const size3& resolution,
    FloatType gridSpacing) :
    pointHashGridSearcher3(
        resolution.x,
        resolution.y,
        resolution.z,
        gridSpacing) 
{
}

pointHashGridSearcher3::pointHashGridSearcher3(
    size_t resolutionX,
    size_t resolutionY,
    size_t resolutionZ,
    FloatType gridSpacing) :
    mGridSpacing(gridSpacing) 
{
    mResolution.x = std::max(static_cast<SSIZE_T>(resolutionX), (SSIZE_T)1);
    mResolution.y = std::max(static_cast<SSIZE_T>(resolutionY), (SSIZE_T)1);
    mResolution.z = std::max(static_cast<SSIZE_T>(resolutionZ), (SSIZE_T)1);
}

pointHashGridSearcher3::pointHashGridSearcher3( const pointHashGridSearcher3& other ) 
{
    set(other);
}

void pointHashGridSearcher3::build( const std::vector<vector3>& points) 
{
    mBuckets.clear();
    mPoints.clear();

    // Allocate memory chuncks
    mBuckets.resize(mResolution.x * mResolution.y * mResolution.z);
    mPoints.resize(points.size());

    if (points.size() == 0) {
        return;
    }

    // Put points into buckets
    for (size_t i = 0; i < points.size(); ++i) {
        mPoints[i] = points[i];
        size_t key = getHashKeyFromPosition(points[i]);
        mBuckets[key].push_back(i);
    }
}

void pointHashGridSearcher3::forEachNearbyPoint(
    const vector3& origin,
    FloatType radius,
    const std::function<void(size_t, const vector3&)>& callback) const 
{
    if (mBuckets.empty()) 
    {
        return;
    }

    size_t nearbyKeys[8];
    getNearbyKeys(origin, nearbyKeys);

    const FloatType queryRadiusSquared = radius * radius;

    for (int i = 0; i < 8; i++) 
    {
        const auto& bucket = mBuckets[nearbyKeys[i]];
        size_t numberOfPointsInBucket = bucket.size();

        for (size_t j = 0; j < numberOfPointsInBucket; ++j) 
        {
            size_t pointIndex = bucket[j];
            FloatType rSquared = (mPoints[pointIndex] - origin).lengthSquared();
            if (rSquared <= queryRadiusSquared) {
                callback(pointIndex, mPoints[pointIndex]);
            }
        }
    }
}

bool pointHashGridSearcher3::hasNearbyPoint(
    const vector3& origin,
    FloatType radius) const {
    if (mBuckets.empty()) {
        return false;
    }

    size_t nearbyKeys[8];
    getNearbyKeys(origin, nearbyKeys);

    const FloatType queryRadiusSquared = radius * radius;

    for (int i = 0; i < 8; i++) {
        const auto& bucket = mBuckets[nearbyKeys[i]];
        size_t numberOfPointsInBucket = bucket.size();

        for (size_t j = 0; j < numberOfPointsInBucket; ++j) {
            size_t pointIndex = bucket[j];
            FloatType rSquared = (mPoints[pointIndex] - origin).lengthSquared();
            if (rSquared <= queryRadiusSquared) {
                return true;
            }
        }
    }

    return false;
}

void pointHashGridSearcher3::add(const vector3& point) 
{
    if (mBuckets.empty()) 
    {
        std::vector<vector3> arr = {point};
        build(arr);
    }
    else 
    {
        size_t i = mPoints.size();
        mPoints.push_back(point);
        size_t key = getHashKeyFromPosition(point);
        mBuckets[key].push_back(i);
    }
}

const std::vector<std::vector<size_t>>&
pointHashGridSearcher3::buckets() const 
{
    return mBuckets;
}

size3 pointHashGridSearcher3::getBucketIndex(const vector3& position) const 
{
    size3 bucketIndex;
    bucketIndex.x = static_cast<SSIZE_T>(
        std::floor(position.x / mGridSpacing));
    bucketIndex.y = static_cast<SSIZE_T>(
        std::floor(position.y / mGridSpacing));
    bucketIndex.z = static_cast<SSIZE_T>(
        std::floor(position.z / mGridSpacing));
    return bucketIndex;
}

size_t pointHashGridSearcher3::getHashKeyFromPosition( const vector3& position) const 
{
    size3 bucketIndex = getBucketIndex(position);
    return getHashKeyFromBucketIndex(bucketIndex);
}

size_t pointHashGridSearcher3::getHashKeyFromBucketIndex( const size3& bucketIndex) const 
{
    size3 wrappedIndex = bucketIndex;
    wrappedIndex.x = bucketIndex.x % mResolution.x;
    wrappedIndex.y = bucketIndex.y % mResolution.y;
    wrappedIndex.z = bucketIndex.z % mResolution.z;
    if (wrappedIndex.x < 0) {
        wrappedIndex.x += mResolution.x;
    }
    if (wrappedIndex.y < 0) {
        wrappedIndex.y += mResolution.y;
    }
    if (wrappedIndex.z < 0) {
        wrappedIndex.z += mResolution.z;
    }
    return static_cast<size_t>(
        (wrappedIndex.z * mResolution.y + wrappedIndex.y) * mResolution.x
        + wrappedIndex.x);
}

void pointHashGridSearcher3::getNearbyKeys( const vector3& position, size_t* nearbyKeys) const
{
    size3 originIndex = getBucketIndex(position), nearbyBucketIndices[8];

    for (int i = 0; i < 8; i++) 
    {
        nearbyBucketIndices[i] = originIndex;
    }

    if (( (FloatType)0.5 + (FloatType)originIndex.x ) * mGridSpacing <= position.x) 
    {
        nearbyBucketIndices[4].x += 1;
        nearbyBucketIndices[5].x += 1;
        nearbyBucketIndices[6].x += 1;
        nearbyBucketIndices[7].x += 1;
    } 
    else 
    {
        nearbyBucketIndices[4].x -= 1;
        nearbyBucketIndices[5].x -= 1;
        nearbyBucketIndices[6].x -= 1;
        nearbyBucketIndices[7].x -= 1;
    }

    if ( ((FloatType)0.5 + (FloatType)originIndex.y ) * mGridSpacing <= position.y)
    {
        nearbyBucketIndices[2].y += 1;
        nearbyBucketIndices[3].y += 1;
        nearbyBucketIndices[6].y += 1;
        nearbyBucketIndices[7].y += 1;
    } 
    else 
    {
        nearbyBucketIndices[2].y -= 1;
        nearbyBucketIndices[3].y -= 1;
        nearbyBucketIndices[6].y -= 1;
        nearbyBucketIndices[7].y -= 1;
    }

    if (((FloatType)0.5 + (FloatType)originIndex.z) * mGridSpacing <= position.z) 
    {
        nearbyBucketIndices[1].z += 1;
        nearbyBucketIndices[3].z += 1;
        nearbyBucketIndices[5].z += 1;
        nearbyBucketIndices[7].z += 1;
    } 
    else 
    {
        nearbyBucketIndices[1].z -= 1;
        nearbyBucketIndices[3].z -= 1;
        nearbyBucketIndices[5].z -= 1;
        nearbyBucketIndices[7].z -= 1;
    }

    for (int i = 0; i < 8; i++) 
    {
        nearbyKeys[i] = getHashKeyFromBucketIndex(nearbyBucketIndices[i]);
    }
}

pointNeighborSearcher3Ptr pointHashGridSearcher3::clone() const
{
    return std::shared_ptr<pointHashGridSearcher3>(
        new pointHashGridSearcher3(*this),
        [] (pointHashGridSearcher3* obj) {
        delete obj; 
    });
}

pointHashGridSearcher3&
pointHashGridSearcher3::operator=(const pointHashGridSearcher3& other) 
{
    set(other);
    return *this;
}

void pointHashGridSearcher3::set(const pointHashGridSearcher3& other) 
{
    mGridSpacing = other.mGridSpacing;
    mResolution = other.mResolution;
    mPoints = other.mPoints;
    mBuckets = other.mBuckets;
}

