//---gridPointGenerator3.h
#ifndef gridPointGenerator3_H
#define gridPointGenerator3_H

#include "pointGenerator3.h"

class gridPointGenerator3 : public pointGenerator3
{
public:
   
    void forEachPoint(
        const boundingBox3& boundingBox,
        FloatType spacing,
        const std::function<bool(const vector3&)>& callback) const;
};

typedef std::shared_ptr< gridPointGenerator3 > gridPointGenerator3Ptr;

#endif

