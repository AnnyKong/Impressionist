/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "filter.h"
#include "forms/filterkerneldialog.h"
#include <iostream>
#include <math.h>
#define Xn 95.047f
#define Yn 100.0f
#define Zn 108.883f

void Filter::ApplyFilterKernel(const unsigned char *source, unsigned char *dest, unsigned int width, unsigned int height, Kernel &k, int offset, bool clamping) {
    // REQUIREMENT: Implement this function
    //translate rgba buffer into three weight by height matrix that record r, g, b, a information
    //these informaiton is for calculate filter value
    int** rGrid = new int*[height];
    int** gGrid = new int*[height];
    int** bGrid = new int*[height];
    int** aGrid = new int*[height];
    for(unsigned int i = 0; i < height; i++){
        rGrid[i] = new int[width];
        gGrid[i] = new int[width];
        bGrid[i] = new int[width];
        aGrid[i] = new int[width];
    }

    int z = -1;
    for(unsigned int i = 0; i < height; i += 1){
        for(unsigned int j = 0; j < width; j += 1){
            rGrid[i][j] = source[++z];
            gGrid[i][j] = source[++z];
            bGrid[i][j] = source[++z];
            aGrid[i][j] = source[++z];
        }
    }
    //above: translate rgb buffer


    //creat destination r, g, b, a grid
    int** new_rGrid = new int*[height];
    int** new_gGrid = new int*[height];
    int** new_bGrid = new int*[height];
    for(unsigned int i = 0; i < height; i++){
        new_rGrid[i] = new int[width]();
        new_gGrid[i] = new int[width]();
        new_bGrid[i] = new int[width]();
    }

    //calculate destination image information

    for(unsigned int i = 0; i < height; i++){//vertical
        for(unsigned int j = 0; j < width; j++){//horizontal

            float newRij = 0.0f, newGij = 0.0f, newBij = 0.0f;
            for(unsigned int h = 0; h  < 5 ; h++){//vertical
                for(unsigned int s = 0; s < 5; s++){//horizontal
                    int x = j + 2 - s, y =  (i - 2 + h);
                    x = x < 0 ? 0 : x;
                    x = x > (width - 1) ? (width - 1): x;
                    y = y < 0 ? 0 : y;
                    y = y > (height - 1) ? (height - 1) : y;
                    newRij += rGrid[y][x] * k.filter_value[s][h];
                    newGij += gGrid[y][x] * k.filter_value[s][h];
                    newBij += bGrid[y][x] * k.filter_value[s][h];
                }
            }

            float offsetf = static_cast<float>(offset);
            newRij += offsetf;
            newGij += offsetf;
            newBij += offsetf;

            new_rGrid[i][j] = newRij < 0 ? 0 :static_cast<int>(newRij);
            new_gGrid[i][j] = newGij < 0 ? 0 :static_cast<int>(newGij);
            new_bGrid[i][j] = newBij < 0 ? 0 :static_cast<int>(newBij);

            new_rGrid[i][j] = newRij > 255 ? 255 : new_rGrid[i][j];
            new_gGrid[i][j] = newGij > 255 ? 255 : new_gGrid[i][j];
            new_bGrid[i][j] = newBij > 255 ? 255 : new_bGrid[i][j];
        }
    }

    unsigned int p = 0;
    for(unsigned int i = 0; i < height; i += 1){
        for(unsigned int j = 0; j < width; j += 1){
            dest[p++] = new_rGrid[i][j];
            dest[p++] = new_gGrid[i][j];
            dest[p++] = new_bGrid[i][j];
            dest[p++] = aGrid[i][j];
        }
    }

}

void Filter::ApplyGaussianBlur(const unsigned char *source, unsigned char *dest, unsigned int width, unsigned int height, float sigma) {
    unsigned int blur_radius = sigma * 3;
    unsigned int kernel_size = blur_radius * 2 + 1;
    // EXTRA CREDIT: Implement this function

}

void Filter::ApplyBilateralMeanBlur(const unsigned char *source, unsigned char *dest, unsigned int width, unsigned int height, unsigned int domain_half_width, unsigned int range) {
    unsigned int kernel_size = domain_half_width * 2 + 1;
    // REQUIREMENT: Implement this function
    int** rGrid = new int*[height];
    int** gGrid = new int*[height];
    int** bGrid = new int*[height];
    int** aGrid = new int*[height];
    for(unsigned int i = 0; i < height; i++){
        rGrid[i] = new int[width];
        gGrid[i] = new int[width];
        bGrid[i] = new int[width];
        aGrid[i] = new int[width];
    }

    int z = -1;
    for(unsigned int i = 0; i < height; i += 1){
        for(unsigned int j = 0; j < width; j += 1){
            rGrid[i][j] = source[++z];
            gGrid[i][j] = source[++z];
            bGrid[i][j] = source[++z];
            aGrid[i][j] = source[++z];
        }
    }
    //above: translate rgb buffer


    //creat destination r, g, b, a grid
    int** new_rGrid = new int*[height];
    int** new_gGrid = new int*[height];
    int** new_bGrid = new int*[height];
    for(unsigned int i = 0; i < height; i++){
        new_rGrid[i] = new int[width]();
        new_gGrid[i] = new int[width]();
        new_bGrid[i] = new int[width]();
    }

    //create kernel
    int** mean_kernel = new int*[kernel_size];
    for(unsigned int i = 0; i < kernel_size; i++){
        mean_kernel[i] = new int[kernel_size]();
    }

    for(unsigned int i = 0; i < height; i++){//vertical
        for(unsigned int j = 0; j < width; j++){//horizontal

            float newRij = 0.0f, newGij = 0.0f, newBij = 0.0f;
            int total_pixel = 0;
            int iR = rGrid[i][j], iG = gGrid[i][j], iB = bGrid[i][j];
            float i_CIE_L, i_CIE_a, i_CIE_b;
            RGBToLab(iR, iG, iB, i_CIE_L, i_CIE_a, i_CIE_b);
            for(unsigned int h = 0; h  < kernel_size ; h++){//vertical
                for(unsigned int s = 0; s < kernel_size; s++){//horizontal
                    int half = kernel_size / 2;
                    int x = j + half - s, y =  (i - half + h);
                    x = x < 0 ? 0 : x;
                    x = x > (width - 1) ? (width - 1): x;
                    y = y < 0 ? 0 : y;
                    y = y > (height - 1) ? (height - 1) : y;
                    // This is the usual way to compute color difference
                    /*int rgb_difference = static_cast<int>(sqrt(((rGrid[y][x] - iR)*(rGrid[y][x] - iR))
                                                +((gGrid[y][x] - iG)*(gGrid[y][x]-iG))
                                                +((bGrid[y][x] - iB)*(bGrid[y][x] - iB))));*/
                    float L, a, b;
                    RGBToLab(rGrid[y][x], gGrid[y][x], bGrid[y][x], L, a, b);
                    int lab_difference = static_cast<int>(sqrt(((L - i_CIE_L)*(L - i_CIE_L))
                                                                 +((a - i_CIE_a)*(a - i_CIE_a))
                                                                 +((b - i_CIE_b)*(b - i_CIE_b))));
                    if (lab_difference <= range) {
                        newRij += rGrid[y][x];
                        newGij += gGrid[y][x];
                        newBij += bGrid[y][x];
                        total_pixel++;
                    }
                }
            }
            new_rGrid[i][j] = static_cast<int> (newRij/total_pixel);
            new_gGrid[i][j] = static_cast<int> (newGij/total_pixel);
            new_bGrid[i][j] = static_cast<int> (newBij/total_pixel);
        }
    }
    int p = 0;
    for(unsigned int i = 0; i < height; i += 1){
        for(unsigned int j = 0; j < width; j += 1){
            dest[p++] = new_rGrid[i][j];
            dest[p++] = new_gGrid[i][j];
            dest[p++] = new_bGrid[i][j];
            dest[p++] = aGrid[i][j];
        }
    }



}

void Filter::ApplyBilateralGaussianBlur(const unsigned char *source, unsigned char *dest, unsigned int width, unsigned int height, float sigma_space, float sigma_range) {
    unsigned int kernel_radius = sigma_space * 3;
    unsigned int kernel_size = kernel_radius * 2 + 1;
    // EXTRA CREDIT: Implement this function
}

float Filter::Gamma(float x){
    return x > 0.04045 ? pow(((x+0.055f)/1.055f), 2.4f) : x / 12.92;
}

void Filter::RGBToLab(int r, int g, int b, float &CIE_L, float &CIE_a, float &CIE_b){
    float R = Gamma(r/255.0f);
    float G = Gamma(g/255.0f);
    float B = Gamma(b/255.0f);
    float X = R*0.4124f + G*0.3576f + B*0.1805f;
    float Y = R*0.2126f + G*0.7152f + B*0.0722f;
    float Z = R*0.0193f + G*0.1192f + B*0.9505f;


    X /= 0.95047f;
    Y /= 1.0f;
    Z /= 1.08883f;
    float FX = X > 0.008856f ? pow(X,1.0f/3.0f) : (7.787f * X +0.137931f);
    float FY = Y > 0.008856f ? pow(Y,1.0f/3.0f) : (7.787f * Y +0.137931f);
    float FZ = Z > 0.008856f ? pow(Z,1.0f/3.0f) : (7.787f * Z +0.137931f);
    CIE_L = Y > 0.008856f ? (116.0f * FY - 16.0f) : (903.3f * Y);
    CIE_a = 500.f * (FX - FY);
    CIE_b = 200.f * (FY - FZ);

}

float Filter::f(float t){
    float thread = (6.0f/29) * (6.0f/29) * (6.0f/29);
    float one_third = 1.0f/3.0f;
    return t > thread ? pow(t, one_third) : (one_third*(29.0f/6)*(29.0f/6)*t + 4.0f/29);
}
