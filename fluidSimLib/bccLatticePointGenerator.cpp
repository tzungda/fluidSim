//---bccLatticePointGenerator.cpp
#include "bccLatticePointGenerator.h"


void bccLatticePointGenerator::forEachPoint(
    const boundingBox3& boundingBox,
    double spacing,
    const std::function<bool(const vector3&)>& callback) const
{
    double halfSpacing = spacing / 2.0;
    double boxWidth = boundingBox.width();
    double boxHeight = boundingBox.height();
    double boxDepth = boundingBox.depth();

    vector3 position;
    bool hasOffset = false;
    bool shouldQuit = false;
    for (int k = 0; k * halfSpacing <= boxDepth && !shouldQuit; ++k) {
        position.z = k * halfSpacing + boundingBox.lowerCorner.z;

        double offset = (hasOffset) ? halfSpacing : 0.0;

        for (int j = 0; j * spacing + offset <= boxHeight && !shouldQuit; ++j) {
            position.y = j * spacing + offset + boundingBox.lowerCorner.y;

            for (int i = 0; i * spacing + offset <= boxWidth; ++i) {
                position.x = i * spacing + offset + boundingBox.lowerCorner.x;
                if (!callback(position)) {
                    shouldQuit = true;
                    break;
                }
            }
        }

        hasOffset = !hasOffset;
    }
}