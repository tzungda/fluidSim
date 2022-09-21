//---common.h
#ifndef common_H
#define common_H

#include <basetsd.h>

/////float type/////
//#define SINGLE_FLOAT
//
#ifdef SINGLE_FLOAT
#define FloatType float
#else
#define FloatType double
#endif
//////////

/////size type/////
#define INT_SIZE
//
#ifdef INT_SIZE
#define SSizeType int
#define SizeType unsigned int
#else
#define SSizeType SSIZE_T
#define SizeType size_t
#endif

#endif

