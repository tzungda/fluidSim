//---pointGenerator3.h
#ifndef pointGenerator3_H
#define pointGenerator3_H

#include <memory>
#include <functional>
#include <vector>

#include "vector3.h"
#include "boundingBox3.h"

class pointGenerator3
{
public:
    pointGenerator3();

    virtual ~pointGenerator3();


    void generate(
        const boundingBox3& boundingBox,
        double spacing,
        std::vector<vector3>* points) const;


    virtual void forEachPoint(
        const boundingBox3& boundingBox,
        double spacing,
        const std::function<bool(const vector3&)>& callback) const = 0;
};

typedef std::shared_ptr<pointGenerator3> pointGenerator3Ptr;

#endif