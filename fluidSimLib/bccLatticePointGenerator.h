//---bccLatticePointGenerator.h
#ifndef bccLatticePointGenerator_H
#define bccLatticePointGenerator_H

#include <memory>

#include "pointGenerator3.h"

// http://en.wikipedia.org/wiki/Cubic_crystal_system
// http://mathworld.wolfram.com/CubicClosePacking.html

class bccLatticePointGenerator : public pointGenerator3
{
    void forEachPoint(
        const boundingBox3& boundingBox,
        double spacing,
        const std::function<bool(const vector3&)>& callback) const override;
};

typedef std::shared_ptr<bccLatticePointGenerator> bccLatticePointGeneratorPtr;

#endif
