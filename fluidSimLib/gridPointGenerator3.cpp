//---gridPointGenerator3.cpp
#include "gridPointGenerator3.h"

void gridPointGenerator3::forEachPoint(
    const boundingBox3& boundingBox,
    double spacing,
    const std::function<bool(const vector3&)>& callback) const
{
    vector3 position;
    double boxWidth = boundingBox.width();
    double boxHeight = boundingBox.height();
    double boxDepth = boundingBox.depth();

    bool shouldQuit = false;
    for (int k = 0; k * spacing <= boxDepth && !shouldQuit; ++k) {
        position.z = k * spacing + boundingBox.lowerCorner.z;

        for (int j = 0; j * spacing <= boxHeight && !shouldQuit; ++j) {
            position.y = j * spacing + boundingBox.lowerCorner.y;

            for (int i = 0; i * spacing <= boxWidth && !shouldQuit; ++i) {
                position.x = i * spacing + boundingBox.lowerCorner.x;
                if (!callback(position)) {
                    shouldQuit = true;
                    break;
                }
            }
        }
    }
}

