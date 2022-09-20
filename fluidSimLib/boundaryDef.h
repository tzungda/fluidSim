//---boundaryDef.h
#ifndef boundaryDef_H
#define boundaryDef_H


// MARK: Common enums

//! No direction.
const int kDirectionNone = 0;

//! Left direction.
const int kDirectionLeft = 1 << 0;

//! RIght direction.
const int kDirectionRight = 1 << 1;

//! Down direction.
const int kDirectionDown = 1 << 2;

//! Up direction.
const int kDirectionUp = 1 << 3;

//! Back direction.
const int kDirectionBack = 1 << 4;

//! Front direction.
const int kDirectionFront = 1 << 5;

//! All direction.
const int kDirectionAll
= kDirectionLeft | kDirectionRight
| kDirectionDown | kDirectionUp
| kDirectionBack | kDirectionFront;

#endif

