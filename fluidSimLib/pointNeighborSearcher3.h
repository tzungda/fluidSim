//---pointNeighborSearcher3.h
#ifndef pointNeighborSearcher3_H
#define pointNeighborSearcher3_H

#include <memory>
#include <functional>
#include <string>
#include <vector>

#include "vector3.h"

class pointNeighborSearcher3
{
public:
    typedef std::function<void(SizeType, const vector3&)>
        ForEachNearbyPointFunc;

    pointNeighborSearcher3();

    virtual ~pointNeighborSearcher3();

    virtual std::string typeName() const = 0;

    virtual void build(const std::vector<vector3>& points) = 0;

    virtual void forEachNearbyPoint(
        const vector3& origin,
        FloatType radius,
        const ForEachNearbyPointFunc& callback) const = 0;


    virtual bool hasNearbyPoint(
        const vector3& origin, FloatType radius) const = 0;


    virtual std::shared_ptr<pointNeighborSearcher3> clone() const = 0;
};

typedef std::shared_ptr<pointNeighborSearcher3> pointNeighborSearcher3Ptr;

#endif
