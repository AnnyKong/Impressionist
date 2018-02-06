#include "singlelinebrush.h"
#include <paintview.h>
#include <qlabeledslider.h>
#include <QFormLayout>
#include <cmath>
#define PI 3.1415926535897
SingleLineBrush::SingleLineBrush(const std::string& name) :
    Brush(name)

{
    thickness_slider_ = new QLabeledSlider;
    thickness_slider_->SetRange(1, 50);


    layout_->addRow("Thickness", thickness_slider_);
    SetThickness(1);
}

void SingleLineBrush::SetThickness(unsigned int thickness){
   thickness = thickness > 50? 50 : thickness;
   thickness_slider_->SetValue(thickness);
}

unsigned int SingleLineBrush::GetThickness() const {
    return thickness_slider_->GetValue();
}

void SingleLineBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void SingleLineBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    UseColor(GetColor(pos));
    int size = GetSize();
    int thickness = GetThickness();
    GLfloat angle =GetAngle() * PI / 180;
    GLfloat dx = size * 0.5f;
    GLfloat dy = thickness * 0.5f;

    // Points to draw
    std::vector<GLfloat> vertex = {
        (GLfloat) pos.x + (size * 0.5f * cos(angle)) - (thickness * 0.5f * sin(angle)), pos.y +(size * 0.5f * sin(angle)) + (thickness * 0.5f * cos(angle)),//right down
        (GLfloat) pos.x + (-size * 0.5f * cos(angle)) - (-thickness * 0.5f * sin(angle)), pos.y +(-size * 0.5f * sin(angle)) + (-thickness * 0.5f * cos(angle)),// left up
        (GLfloat) pos.x + (size * 0.5f * cos(angle)) - (-thickness * 0.5f * sin(angle)), pos.y +(size * 0.5f * sin(angle)) + (-thickness * 0.5f * cos(angle)),// right up
    };

    std::vector<GLfloat> vertex2 = {
          (GLfloat) pos.x + (-size * 0.5f * cos(angle)) - (thickness * 0.5f * sin(angle)), pos.y +(-size * 0.5f * sin(angle)) + (thickness * 0.5f * cos(angle)),// left down
          (GLfloat) pos.x + (size * 0.5f * cos(angle)) - (thickness * 0.5f * sin(angle)), pos.y +(size * 0.5f * sin(angle)) + (thickness * 0.5f * cos(angle)),//right down
          (GLfloat) pos.x + (-size * 0.5f * cos(angle)) - (-thickness * 0.5f * sin(angle)), pos.y +(-size * 0.5f * sin(angle)) + (-thickness * 0.5f * cos(angle)),// left up
    };

    // Upload the vertex data to GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Upload the vertex data to GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex2.size(), vertex2.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
void SingleLineBrush::BrushEnd(const glm::vec2 pos) {

}
