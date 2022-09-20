//---pointGenerator3.cpp

#include "pointGenerator3.h"

pointGenerator3::pointGenerator3()
{
}

pointGenerator3::~pointGenerator3()
{
}

void pointGenerator3::generate(
    const boundingBox3& boundingBox,
    double spacing,
    std::vector<vector3>* points) const
{
    forEachPoint(
        boundingBox,
        spacing,
        [&points](const vector3& point) {
            points->push_back(point);
            return true;
        });
}