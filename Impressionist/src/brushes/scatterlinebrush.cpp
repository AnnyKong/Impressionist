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
#include "scatterlinebrush.h"
#include <paintview.h>
#include <qlabeledslider.h>
#include <QFormLayout>
#include <cmath>


ScatterLineBrush::ScatterLineBrush(const std::string& name) :
    Brush(name),
    radious_slider_(new QLabeledSlider),
    density_slider_(new QLabeledSlider),
    thickness_slider_(new QLabeledSlider)
{
    radious_slider_->SetRange(1, 50);
    density_slider_->SetRange(1, 20);
    thickness_slider_->SetRange(1, 50);
    layout_->addRow("Radious", radious_slider_);
    layout_->addRow("Density", density_slider_);
    layout_->addRow("Thickness", thickness_slider_);
    //Set Default Size
    SetRadious(10);
    SetDensity(3);
    SetThickness(1);
}

void ScatterLineBrush::SetThickness(unsigned int thickness){
   thickness = thickness > 50? 50 : thickness;
   thickness_slider_->SetValue(thickness);
}

void ScatterLineBrush::SetRadious(unsigned int rad){
    rad = rad > 50 ? 50 : rad;
    radious_slider_->SetValue(rad);
}

void ScatterLineBrush::SetDensity(unsigned int density){
    density = density > 50 ? 50 : density;
    density_slider_->SetValue(density);
}

unsigned int ScatterLineBrush::GetRadious() const{
    return radious_slider_->GetValue();
}

unsigned int ScatterLineBrush::GetDensity() const{
    return density_slider_->GetValue();
}

unsigned int ScatterLineBrush::GetThickness() const {
    return thickness_slider_->GetValue();
}

void ScatterLineBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void ScatterLineBrush::BrushMove(const glm::vec2 pos) {
    // Set the color



    int radious = GetRadious();
    int density = GetDensity();
    int size = GetSize();
    int thickness = GetThickness();
    GLfloat angle =GetAngle() * PI / 180;

    // Points to draw
    for(int i = 0; i < density; i++){
        double f = (double)rand() / RAND_MAX;
        double random = radious * (-1) + f * (2 * radious);
        int rad_angle = (rand() % 360) * PI / 180;
        GLfloat pointX = pos.x + (GLfloat)(cos(rad_angle) * random);
        GLfloat pointY = pos.y + (GLfloat)(sin(rad_angle) * random);
        std::vector<GLfloat> vertex = {
          //pos.x + (-dx * cos(angle)) - (dy * sin(angle)), pos.y +(-dx * sin(angle)) + (dy * cos(angle)),// left down
            (GLfloat) pointX + (size * 0.5f * cos(angle)) - (thickness * 0.5f * sin(angle)), pointY +(size * 0.5f * sin(angle)) + (thickness * 0.5f * cos(angle)),//right down
            (GLfloat) pointX + (-size * 0.5f * cos(angle)) - (-thickness * 0.5f * sin(angle)), pointY +(-size * 0.5f * sin(angle)) + (-thickness * 0.5f * cos(angle)),// left up
            (GLfloat) pointX + (size * 0.5f * cos(angle)) - (-thickness * 0.5f * sin(angle)), pointY +(size * 0.5f * sin(angle)) + (-thickness * 0.5f * cos(angle)),// right up
        };

        std::vector<GLfloat> vertex2 = {
              (GLfloat) pointX + (-size * 0.5f * cos(angle)) - (thickness * 0.5f * sin(angle)), pointY +(-size * 0.5f * sin(angle)) + (thickness * 0.5f * cos(angle)),// left down
              (GLfloat) pointX + (size * 0.5f * cos(angle)) - (thickness * 0.5f * sin(angle)), pointY +(size * 0.5f * sin(angle)) + (thickness * 0.5f * cos(angle)),//right down
              (GLfloat) pointX + (-size * 0.5f * cos(angle)) - (-thickness * 0.5f * sin(angle)), pointY +(-size * 0.5f * sin(angle)) + (-thickness * 0.5f * cos(angle)),// left up
            //pos.x + (dx * cos(angle)) - (-dy * sin(angle)), pos.y +(dx * sin(angle)) + (-dy * cos(angle)),// right up
        };

        glm::vec2 newpos ={pointX, pointY};
        UseColor(GetColor(newpos));
        // Upload the vertex data to GPU buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STREAM_DRAW);

        // Set the size of the points and draw them
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Upload the vertex data to GPU buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex2.size(), vertex2.data(), GL_STREAM_DRAW);

        // Set the size of the points and draw them
        glDrawArrays(GL_TRIANGLES, 0, 3);

    }
}

void ScatterLineBrush::BrushEnd(const glm::vec2 pos) {
}
