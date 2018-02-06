#include "starbrush.h"
#include <paintview.h>
#include <qlabeledslider.h>
#include <QFormLayout>
#include <math.h>
StarBrush::StarBrush(const std::string& name) :
    Brush(name)

{
    height_slider_ = new QLabeledSlider;
    height_slider_->SetRange(1, 50);
    layout_->addRow("Height", height_slider_);
    SetHeight(25);
}

void StarBrush::SetHeight(unsigned int height){
   height = height > 50? 50 : height;
   height_slider_->SetValue(height);
}

unsigned int StarBrush::GetHeight() const {
    return height_slider_->GetValue();
}

void StarBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void StarBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    UseColor(GetColor(pos));
    int size = GetSize();
    int height =GetHeight();
    GLfloat angle =GetAngle() * M_PI / 180;
    // Points to draw
    std::vector<GLfloat> vertexPoint = {
        pos.x, pos.y
    };
    std::vector<GLfloat> vertex1 = {
        pos.x +(-cos(angle)*size*0.5f + sin(angle)*size*0.5f), pos.y + (-sin(angle)*size*0.5f - cos(angle)*size*0.5f),
        pos.x +(cos(angle)*size*0.5f + sin(angle)*size*0.5f), pos.y +(sin(angle)*size*0.5f - cos(angle)*size*0.5f),
        pos.x +(sin(angle) * 0.5f*size + sin(angle)*height), pos.y +(-cos(angle) * 0.5f *size - cos(angle) * height)
    };
    std::vector<GLfloat> vertex2 = {
        pos.x  + (-cos(angle)*size*0.5f + sin(angle)*size*0.5f), pos.y + (-sin(angle)*size*0.5f - cos(angle)*size*0.5f),
        pos.x + (-cos(angle)*size*0.5f - sin(angle)*size*0.5f), pos.y + (-sin(angle)*size*0.5f + cos(angle)*size*0.5f),
        pos.x + (-cos(angle) * 0.5f*size - cos(angle)*height) , pos.y + (-sin(angle) * 0.5f*size - sin(angle)*height)
    };

    std::vector<GLfloat> vertex3 = {
        pos.x + (-cos(angle)*size*0.5f - sin(angle)*size*0.5f), pos.y + (-sin(angle)*size*0.5f + cos(angle)*size*0.5f),
        pos.x + (cos(angle)*size*0.5f - sin(angle)*size*0.5f), pos.y + (sin(angle)*size*0.5f + cos(angle)*size*0.5f),
        pos.x + (-sin(angle) * 0.5f*size - sin(angle)*height), pos.y + (cos(angle) * 0.5f*size + cos(angle)*height)
    };

    std::vector<GLfloat> vertex4 = {
        pos.x + (cos(angle)*size*0.5f + sin(angle)*size*0.5f), pos.y + (sin(angle)*size*0.5f - cos(angle)*size*0.5f),
        pos.x + (cos(angle)*size*0.5f - sin(angle)*size*0.5f), pos.y + (sin(angle)*size*0.5f + cos(angle)*size*0.5f),
        pos.x + (cos(angle) * 0.5f*size + cos(angle)*height), pos.y + (sin(angle) * 0.5f*size + sin(angle)*height)
    };

    // Upload the vertex data to GPU buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexPoint.size(), vertexPoint.data(), GL_STREAM_DRAW);

    // Set the size of the points and draw them
    glPointSize(GetSize());
    glDrawArrays(GL_POINTS, 0, 1);


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
void StarBrush::BrushEnd(const glm::vec2 pos) {

}
