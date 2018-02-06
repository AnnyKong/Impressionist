/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef RGBABUFFER_H
#define RGBABUFFER_H

#include <cstring>

// Simple wrapper around an RGBA32 uchar buffer
class RGBABuffer {
public:
    RGBABuffer(unsigned int width, unsigned int height) :
        Size(width * height * 4),
        Width(width),
        Height(height),
        Bytes(new unsigned char[Size]) { }

    ~RGBABuffer() { delete[] Bytes; }

    const unsigned int Size;
    const unsigned int Width;
    const unsigned int Height;
    unsigned char* Bytes;
};

#endif // UCHARBUFFER_H
