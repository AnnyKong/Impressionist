#include "windwheelbrush.h"
#include <paintview.h>
#include <qlabeledslider.h>
#include <QFormLayout>
#include <math.h>
WindWheelBrush::WindWheelBrush(const std::string& name) :
    Brush(name)

{
    width_slider_ = new QLabeledSlider;
    width_slider_->SetRange(1, 40);
    layout_->addRow("Width", width_slider_);
    SetWidth(5);
}

void WindWheelBrush::SetWidth(unsigned int width){
   width = width > 50? 50 : width;
   width_slider_->SetValue(width);
}

unsigned int WindWheelBrush::GetWidth() const {
    return width_slider_->GetValue();
}

void WindWheelBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void WindWheelBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    UseColor(GetColor(pos));
    int size = GetSize();
    int width =GetWidth();
    GLfloat angle = GetAngle() * M_PI / 180;

    std::vector<GLfloat> vertex1 = {
        pos.x, pos.y,
        pos.x +(-cos(angle)*0.5f * width + sin(angle)*0.5f*size), pos.y + (-sin(angle)*0.5f*width - cos(angle) *0.5f *size),
        pos.x +(cos(angle)*0.5f * width + sin(angle)*0.5f*size), pos.y + (sin(angle)*0.5f*width - cos(angle) *0.5f *size)
    };
    std::vector<GLfloat> vertex2 = {
        pos.x, pos.y,
        pos.x +(-cos(angle)*0.5f * size + sin(angle)*0.5f*width), pos.y + (-sin(angle)*0.5f*size - cos(angle) *0.5f *width),
        pos.x +(-cos(angle)*0.5f * size - sin(angle)*0.5f*width), pos.y + (-sin(angle)*0.5f*size + cos(angle) *0.5f *width)
    };

    std::vector<GLfloat> vertex3 = {
        pos.x, pos.y,
        pos.x + (-cos(angle)*0.5f * width - sin(angle)*0.5f*size), pos.y + (-sin(angle)*0.5f*width + cos(angle) *0.5f *size),
        pos.x + (cos(angle)*0.5f * width - sin(angle)*0.5f*size), pos.y + (sin(angle)*0.5f*width + cos(angle) *0.5f *size)
    };

    std::vector<GLfloat> vertex4 = {
        pos.x, pos.y,
        pos.x +(cos(angle)*0.5f * size + sin(angle)*0.5f*width), pos.y + (sin(angle)*0.5f*size - cos(angle) *0.5f *width),
        pos.x +(cos(angle)*0.5f * size - sin(angle)*0.5f*width), pos.y + (sin(angle)*0.5f*size + cos(angle) *0.5f *width)
    };


    // Upload the vertex data to GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex1.size(), vertex1.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Upload the vertex data to GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex2.size(), vertex2.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex3.size(), vertex3.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex4.size(), vertex4.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
void WindWheelBrush::BrushEnd(const glm::vec2 pos) {

}
