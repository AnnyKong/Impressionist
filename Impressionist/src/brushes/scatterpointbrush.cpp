/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/
#define PI 3.1415926535897
#include "scatterpointbrush.h"
#include <paintview.h>
#include <qlabeledslider.h>
#include <QFormLayout>
#include <cmath>

ScatterPointBrush::ScatterPointBrush(const std::string& name) :
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

void ScatterPointBrush::SetRadious(unsigned int rad){
    rad = rad > 50 ? 50 : rad;
    radious_slider_->SetValue(rad);
}

void ScatterPointBrush::SetDensity(unsigned int density){
    density = density > 50 ? 50 : density;
    density_slider_->SetValue(density);
}

unsigned int ScatterPointBrush::GetRadious() const{
    return radious_slider_->GetValue();
}

unsigned int ScatterPointBrush::GetDensity() const{
    return density_slider_->GetValue();
}

void ScatterPointBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void ScatterPointBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    //UseColor(GetColor(pos));

    int radious = GetRadious();
    int density = GetDensity();

    // Points to draw
    for(int i = 0; i < density; i++){
        double f = (double)rand() / RAND_MAX;
        double random = radious * (-1) + f * (2 * radious);
        int angle = (rand() % 360) * PI / 180;
        GLfloat pointX = pos.x + (GLfloat)(cos(angle) * random);
        GLfloat pointY = pos.y + (GLfloat)(sin(angle) * random);
        std::vector<GLfloat> vertex = {
            pointX, pointY
        };
        glm::vec2 newpos ={pointX, pointY};
        UseColor(GetColor(newpos));
        // Upload the vertex data to GPU buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STREAM_DRAW);

        // Set the size of the points and draw them
        glPointSize(GetSize());
        glDrawArrays(GL_POINTS, 0, 1);
    }
}

void ScatterPointBrush::BrushEnd(const glm::vec2 pos) {

}
