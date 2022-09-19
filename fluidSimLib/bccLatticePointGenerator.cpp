//---bccLatticePointGenerator.cpp
#include "bccLatticePointGenerator.h"


void bccLatticePointGenerator::forEachPoint(
    const boundingBox3& boundingBox,
    FloatType spacing,
    const std::function<bool(const vector3&)>& callback) const
{
    FloatType halfSpacing = spacing / (FloatType)2.0 ;
    FloatType boxWidth = boundingBox.width();
    FloatType boxHeight = boundingBox.height();
    FloatType boxDepth = boundingBox.depth();

    vector3 position;
    bool hasOffset = false;
    bool shouldQuit = false;
    for (int k = 0; k * halfSpacing <= boxDepth && !shouldQuit; ++k) {
        position.z = k * halfSpacing + boundingBox.lowerCorner.z;

        FloatType offset = (FloatType)( (hasOffset) ? halfSpacing : 0.0 );

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