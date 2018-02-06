/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#ifndef FILTER_H
#define FILTER_H

#include <vectors.h>
#include <functional>
#include <forms/filterkerneldialog.h>
#include <rgbabuffer.h>
#include <glinclude.h>
#include <QDebug>
#include<vectors.h>


// Utility class for applying filters
// OPTIONAL: You can choose to implement this as a helper class for filter kernel.
class Kernel {
public:
    Kernel::Kernel() {
        filter_value = new float*[5];
        for(int i = 0; i < 5; i++){
            filter_value[i] = new float[5];
        }
    }
    float** filter_value;
    float divisor;
    ~Kernel() {delete[] filter_value;}
};

class Filter {
public:
    // Applies a filter kernel to the RGB channels of the source image and stores it into dest
    static void ApplyFilterKernel(const unsigned char* source, unsigned char* dest, unsigned int width, unsigned int height, Kernel& k, int offset = 0, bool clamping = true);

    // Applies a gaussian blur to the RGB channels of the source image and stores it into dest
    static void ApplyGaussianBlur(const unsigned char* source, unsigned char* dest, unsigned int width, unsigned int height, float sigma = 1);

    // Applies a bilateral mean blur to the RGB channels of the source image and stores it into dest
    static void ApplyBilateralMeanBlur(const unsigned char* source, unsigned char* dest, unsigned int width, unsigned int height, unsigned int domain_half_width, unsigned int range);

    // Applies a bilateral gaussian blur to the RGB channels of the source image and stores it into dest
    static void ApplyBilateralGaussianBlur(const unsigned char* source, unsigned char* dest, unsigned int width, unsigned int height, float sigma_space, float sigma_range);

protected:
    static float Filter::Gamma(float x);
    static float Filter::f(float t);
    static void Filter::RGBToLab(int r, int g, int b, float &CIE_L, float &CIE_a, float &CIE_b);
};

#endif // FILTER_H
