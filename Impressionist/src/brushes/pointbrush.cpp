/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "pointbrush.h"
#include <paintview.h>

PointBrush::PointBrush(const std::string& name) :
    Brush(name)
{
}

void PointBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void PointBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    UseColor(GetColor(pos));

    // Points to draw
    std::vector<GLfloat> vertex = {
        pos.x, pos.y
    };

    // Upload the vertex data to GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glPointSize(GetSize());
    glDrawArrays(GL_POINTS, 0, 1);
}

void PointBrush::BrushEnd(const glm::vec2 pos) {
}
