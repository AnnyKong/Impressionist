/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "scattercirclebrush.h"
#include <paintview.h>
#include <QFormLayout>
#include <qlabeledslider.h>
#include <math.h>

ScatterCircleBrush::ScatterCircleBrush(const std::string& name) :
    Brush(name),
    radious_slider_(new QLabeledSlider),
    density_slider_(new QLabeledSlider)
{
    radious_slider_->SetRange(1, 50);
    density_slider_->SetRange(1, 20);
    layout_->addRow("Radious", radious_slider_);
    layout_->addRow("Density", density_slider_);
    //Set Default Size
    SetRadious(10);
    SetDensity(3);
}

void ScatterCircleBrush::SetRadious(unsigned int rad){
    rad = rad > 50 ? 50 : rad;
    radious_slider_->SetValue(rad);
}

void ScatterCircleBrush::SetDensity(unsigned int density){
    density = density > 50 ? 50 : density;
    density_slider_->SetValue(density);
}

unsigned int ScatterCircleBrush::GetRadious() const{
    return radious_slider_->GetValue();
}

unsigned int ScatterCircleBrush::GetDensity() const{
    return density_slider_->GetValue();
}

void ScatterCircleBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void ScatterCircleBrush::BrushMove(const glm::vec2 pos) {
    // Set the color


    int radious = GetRadious();
    int density = GetDensity();

    // Points to draw
    for(int i = 0; i < density; i++){
        double f = (double)rand() / RAND_MAX;
        double random = radious * (-1) + f * (2 * radious);
        int angle = (rand() % 360) * M_PI / 180;

        int size = GetSize();
        GLfloat circleRadious = size * 0.5f;
        std::vector<GLfloat> vertex(724);
        GLfloat pointX = pos.x + (GLfloat)(cos(angle) * random);
        GLfloat pointY = pos.y + (GLfloat)(sin(angle) * random);
        vertex[0] = pointX;
        vertex[1] = pointY;
        // Points to draw
        for(int i = 2; i < 724; i+=2){
            GLfloat angle = (GLfloat) ((i - 2)/2) * M_PI/180;
            vertex[i] = pointX + circleRadious * cos(angle);
            vertex[i + 1] = pointY + circleRadious * sin(angle);
        }
        glm::vec2 newpos ={pointX, pointY};
        UseColor(GetColor(newpos));
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STREAM_DRAW);

        // Set the size of the points and draw them
        glDrawArrays(GL_TRIANGLE_FAN, 0, 362);
    }
}

void ScatterCircleBrush::BrushEnd(const glm::vec2 pos) {
}
