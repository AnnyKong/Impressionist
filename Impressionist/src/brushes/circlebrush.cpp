/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "circlebrush.h"
#include <paintview.h>
#include <qlabeledslider.h>
#include <math.h>

CircleBrush::CircleBrush(const std::string& name) :
    Brush(name)
{

}

void CircleBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}


void CircleBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    UseColor(GetColor(pos));
    //get the size
    int size = GetSize();
    GLfloat radious = size * 0.5f;
    std::vector<GLfloat> vertex(724);
    vertex[0] = pos.x;
    vertex[1] = pos.y;
    // Points to draw
    for(int i = 2; i < 724; i+=2){
        GLfloat angle = (GLfloat) ((i - 2)/2) * M_PI/180;
        vertex[i] = pos.x + radious * cos(angle);
        vertex[i + 1] = pos.y + radious * sin(angle);
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLE_FAN, 0, 362);\
}

void CircleBrush::BrushEnd(const glm::vec2 pos) {
}
