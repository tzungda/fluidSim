//---fdmLinearSystemSolver3.h
#ifndef fdmLinearSystemSolver3_H
#define fdmLinearSystemSolver3_H

#include <memory>

#include "fdmLinearSystem3.h"

class fdmLinearSystemSolver3
{
public:
    virtual bool solve(fdmLinearSystem3 *system) = 0;
};

typedef std::shared_ptr<fdmLinearSystemSolver3> fdmLinearSystemSolver3Ptr;

#endif
