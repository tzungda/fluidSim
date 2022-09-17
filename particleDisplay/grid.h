//---grid.h
#ifndef grid_H
#define grid_H

#include "glad.h"

#include <vector>

class Grid
{
private:
    std::vector<GLuint> buffers;
    GLuint vao;
    GLsizei nVerts;

public:
    Grid( float size = 10.0f, int nDivisions = 10 );
    virtual ~Grid();
    void render() const ;
};

#endif