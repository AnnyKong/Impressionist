/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "brush.h"
#include <filters/filter.h>
#include <QWidget>
#include <QFormLayout>
#include <QComboBox>
#include <qlabeledslider.h>
#include <iostream>
#include <math.h>

Brush::Brush(const std::string& name) :
    widget_(new QWidget),
    layout_(new QFormLayout),
    name_(name),
    size_slider_(new QLabeledSlider),
    o_slider_(new QLabeledSlider),
    angle_slider_(new QLabeledSlider),
    angle_mode_(AngleMode::Direct),
    color_(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
    color_image_(nullptr),
    color_image_width_(0),
    color_image_height_(0),
    color_mode_(ColorMode::Solid),
    color_location_(-1)

{
    widget_->setLayout(layout_);

    // Size Slider
    size_slider_->SetRange(1, 100);
    layout_->addRow("Size", size_slider_);

    // REQUIREMENT: Allow user to control the opacity via a slider
    o_slider_->SetRange(1, 100);
    layout_->addRow("Opacity", o_slider_);
    angle_slider_->SetRange(0, 360);
    layout_->addRow("Angle", angle_slider_);
    previous_angle = 180;
    // Default Values
    SetSize(15);
    SetO(50);
}

QWidget* Brush::GetWidget() const {
    return widget_;
}

std::string Brush::GetName() const {
    return name_;
}


void Brush::SetSize(unsigned int size) {
    size = size > 100 ? 100 : size;
    size_slider_->SetValue(size);
}

void Brush::SetAngleMode(AngleMode angle_mode){
   // angle_slider_->SetValue(60);
   angle_mode_ = angle_mode;
}

void Brush::SetO(unsigned int O) {
    O = O > 100 ? 100 : O;
    o_slider_->SetValue(O);
}
void Brush::SetColor(const glm::vec3& color) {
    color_ = color;
}

void Brush::SetColorImage(const unsigned char* color_image, unsigned int width, unsigned int height) {
    color_image_ = color_image;
    color_image_width_ = width;
    color_image_height_ = height;
}

void Brush::SetColorMode(ColorMode color_mode) {
    color_mode_ = color_mode;
}

void Brush::SetColorLocation(GLint color_location) {
    color_location_ = color_location;
}

void Brush::UseColor(const glm::vec4& color) {
    assert(color_location_ != -1);
    glUniform4fv(color_location_, 1, glm::value_ptr(color));
}

unsigned int Brush::GetSize() const {
    return size_slider_->GetValue();
}

glm::vec4 Brush::GetColor(glm::ivec2 position) {
    if (color_mode_ == ColorMode::Sample && color_image_ != nullptr) {
        unsigned int width = color_image_width_;
        unsigned int height = color_image_height_;

        // Don't exceed the coordinates of the color image
        if (position.x < 0) position.x = 0;
        else if (position.x >= width) position.x = width - 1;
        if (position.y < 0) position.y = 0;
        else if (position.y >= height) position.y = height - 1;

        // Sample the color image at the position
        glm::vec4 color;
        int index = (position.y * 4) * width + (position.x * 4);
        color.r = color_image_[index++] / 255.0f;
        color.g = color_image_[index++] / 255.0f;
        color.b = color_image_[index++] / 255.0f;
       // color.a = color_image_[index++] / 255.0f;
        color.a = color_image_[index++]*(getOpacity()/100.0f) / 255.0f;
        return color;
    } else {
        return glm::vec4(color_, 1.0f);
    }
}

unsigned int Brush::GetAngle() {
    return angle_slider_->GetValue();
}

void Brush::SetAngleStart(const glm::vec2 startPos){
    float x = startPos.x;
    float y = startPos.y;
    angle_control_start = glm::vec2(x, y);
}

void Brush::SetAngleEnd(const glm::vec2 endPos) {
    float x = endPos.x;
    float y = endPos.y;
    angle_control_end = glm::vec2(x, y);
    GLfloat radianAngle = atan2(angle_control_end.y - angle_control_start.y, angle_control_end.x - angle_control_start.x);
    radianAngle+=M_PI;
    int angle = (int)(radianAngle*180/M_PI);
    angle_slider_->SetValue(angle);
    int size =(int)sqrt(pow((double)y - angle_control_start.y, 2) + pow((double)x - angle_control_start.x, 2));
    size_slider_->SetValue(size > 100 ? 100 : size);
}


void Brush::SetMousePre(const glm::vec2 pre){
    float x = pre.x;
    float y = pre.y;
    MousePre = glm::vec2(x, y);
}

void Brush::SetMouseCur(const glm::vec2 post){
    float x = post.x;
    float y = post.y;
    MouseCur = glm::vec2(x, y);
}
AngleMode Brush::getAngleMode() {
    return angle_mode_;
}

void Brush::SetAngle(int angle)
 {
    angle = angle>360? 360:angle;
    angle_slider_ ->SetValue( angle);
}

int Brush::getOpacity() {
    return o_slider_->GetValue();
}
